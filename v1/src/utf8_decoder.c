#include "utf8.h"


static char HEXA_MAP[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static char* bainary_to_hex(char byte) {
    static char hex[3];
    hex[0] = HEXA_MAP[(byte & 0xF0) >> 4];
    hex[1] = HEXA_MAP[byte & 0x0F];
    hex[2] = '\0';
    return hex;
}

int utf8_char_length(uint8_t lead) {
    if ((lead & 0x80) == 0x00) return 1;
    else if ((lead & 0xE0) == 0xC0) return 2;
    else if ((lead & 0xF0) == 0xE0) return 3;
    else if ((lead & 0xF8) == 0xF0) return 4;
    else return -1;
}

static uint8_t extract_utf8_bits(uint8_t byte, short usable_bits, short prev_byte_len) {
    if (usable_bits == prev_byte_len) return (byte & (0xFF >> 4)) << 4;
    if (usable_bits > prev_byte_len) return (byte & (0xFF >> 5)) << 3;
    else return (byte & (0xFF >> 6)) << 6;
}

static char* utf8_to_unicode(char* sequence, int start) {
    short bytes_count = utf8_char_length((uint8_t)sequence[start]);
    if (bytes_count == -1) return NULL;

    char* unicode = (char*)malloc(sizeof(char) * 7);
    if (unicode == NULL) return NULL;
    for (int i = 0; i < 6; i++) unicode[i] = '0';

    int p = 5;

    int8_t prev = -1;
    short prev_byte_len = 0;

    for (int i = start + bytes_count - 1; i >= start && sequence[i] != '\0'; i--) {
        if (prev == -1) {
            uint8_t trim_byte = (sequence[i] & (0xFF >> 2));
            prev = trim_byte;
            prev_byte_len = 6;
        }
        else {
            uint8_t trim_byte = (i == start ?
                extract_utf8_bits(sequence[i], 8 - (bytes_count + 1), prev_byte_len) : ((sequence[i] & (0xFF >> prev_byte_len)) << prev_byte_len)
                ) | prev;
            char* hex = bainary_to_hex(trim_byte);
            unicode[p--] = hex[1];
            if (p >= 0) unicode[p--] = hex[0];

            if (i == start && (8 - (bytes_count + 1)) < prev_byte_len) {
                trim_byte = ((sequence[i] & (0xFF >> 3)) >> 2);
                hex = bainary_to_hex(trim_byte);
                unicode[p--] = hex[1];
                if (p >= 0) unicode[p--] = hex[0];
            }
            else {
                prev = (sequence[i] & (0xFF >> 2)) >> ((8 - prev_byte_len));
                prev_byte_len -= 2;
            }
        }
    }

    unicode[6] = '\0';

    return unicode;
}

char** utf8_decode(char* str) {
    size_t length = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] & 0x80) != 0x00) {
            int lead_bit = i;
            while (str[i] != '\0' && (str[lead_bit] & (0b10000000 >> (i - lead_bit))) != 0) {
                i++;
                if (i - lead_bit == 5) break;
            }
        }

        length++;

        if (str[i] == '\0') break;
    }

    char** unicode_list = (char**)malloc(sizeof(char*) * (length + 1));
    if (unicode_list == NULL) return NULL;
    size_t count = 0;
    for (int i = 0; str[i] != '\0'; i++) {

        if ((str[i] & 0x80) == 0x00) { // ASCII
            char* hexa = bainary_to_hex(str[i]);
            unicode_list[count] = (char*)malloc(sizeof(char) * 7);
            if (unicode_list[count] == NULL) return NULL;
            snprintf(unicode_list[count], 7, "0000%s", hexa);

        }
        else {
            size_t temp = 0;

            int lead_bit = i;
            int bytes_count = utf8_char_length(str[i]);



            char* unicode = utf8_to_unicode(str, i);
            if (unicode == NULL) return NULL;

            i += bytes_count - 1;
            unicode_list[count] = unicode;
        }
        count++;
    }

    unicode_list[length] = NULL;

    return unicode_list;
}
