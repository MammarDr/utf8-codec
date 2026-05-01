#include "utf8.h";

static int hex_value(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
	if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
	return -1; 
}

static uint8_t get_byte_from_pair(char* unicode, size_t index) {
	if (index == 0) return (hex_value(unicode[4]) << 4) | hex_value(unicode[5]);
	if (index == 1) return (hex_value(unicode[2]) << 4) | hex_value(unicode[3]);
	if (index == 2) return (hex_value(unicode[0]) << 4) | hex_value(unicode[1]);
	return -1;
}

static uint8_t* hexa_to_utf8(char* unicode, int* leng) {

	int bytes_count = bytes_count_from_unicode(unicode);
	if (bytes_count == -1) return NULL;

	uint8_t* utf8 = (uint8_t*)malloc(sizeof(uint8_t) * (bytes_count + 1));
	size_t count = 0;
	if (bytes_count == 1) {
		utf8[0] = get_byte_from_pair(unicode, 0);
		*leng = 1;
	}
	
	if (bytes_count == 2) {
		utf8[1] = 0x80 | (get_byte_from_pair(unicode, 0) & (0xFF >> 2));
		utf8[0] = 0xC0 | ((get_byte_from_pair(unicode, 0) & (0xFF << 6)) >> 6) | ((get_byte_from_pair(unicode, 1) & (0xFF >> 4)) << 2);
		*leng = get_byte_from_pair(unicode, 1) == 0 ? 1 : 2;
	}

	if (bytes_count == 3) {
		utf8[2] = 0x80 | (get_byte_from_pair(unicode, 0) & (0xFF >> 2));
		utf8[1] = 0x80 | (((get_byte_from_pair(unicode, 0) & (0xFF << 6)) >> 6) | ((get_byte_from_pair(unicode, 1) & (0xFF >> 4)) << 2));
		utf8[0] = 0xE0 | ((get_byte_from_pair(unicode, 1) & (0xFF << 4)) >> 4);
		*leng = 2;
	}

	if (bytes_count == 4) {
		utf8[3] = 0x80 | (get_byte_from_pair(unicode, 0) & (0xFF >> 2));
		utf8[2] = 0x80 | (((get_byte_from_pair(unicode, 0) & (0xFF << 6)) >> 6) | ((get_byte_from_pair(unicode, 1) & (0xFF >> 4)) << 2));
		utf8[1] = 0x80 | (((get_byte_from_pair(unicode, 1) & (0xFF << 4)) >> 4) | ((get_byte_from_pair(unicode, 2) & (0xFF >> 5)) << 4));
		utf8[0] = 0xF0 | ((get_byte_from_pair(unicode, 2) & 0b00011100) >> 2);
		*leng = 3;
	}

	utf8[bytes_count] = ((void*)0);
	
	return utf8;
}

static int bytes_count_from_unicode(char* unicode) {
	int size = strlen(unicode);
	if (size > 6) return -1;
	if (size > 4 && (unicode[1] != '0' || unicode[0] != '0')) return 4;
	if (size > 2 && (unicode[3] >= '8' || unicode[2] != '0')) return 3;
	if (size > 1 && (unicode[4] >= '8' || unicode[3] != '0')) return 2;
	return 1;

}

static char* normalize_unicode(char* unicode) {
	int size = strlen(unicode);

	if (size == 6) return unicode;
	if (size > 6) return unicode + (size - 6);

	int extra = 6 - size;
	char* normalized_unicode = (char*)malloc(size + extra);
	for (int i = 0; i < extra; i++) normalized_unicode[i] = '0';
	strcpy(normalized_unicode + extra, unicode);

	return normalized_unicode;

}

uint8_t* utf8_encode(char** unicode) {
	if (unicode == NULL) return NULL;
	size_t size = 64;
	size_t count = 0;
	uint8_t* utf8 = (uint8_t*)malloc(sizeof(uint8_t) * size);
	if (utf8 == NULL) return NULL;

	for (int i = 0; unicode[i] != NULL; i++) {
		unicode[i] = normalize_unicode(unicode[i]);
		int skip = 0;
		uint8_t* res = hexa_to_utf8(unicode[i], &skip);
		if (res == NULL) {
			free(utf8);
			return NULL;
		}
		if ((count + 1) + skip >= size) {
			size = size * 2;
			uint8_t* temp = (uint8_t*)realloc(utf8, size);
			if (temp == NULL) {
				free(res);
				free(utf8);
				return NULL;
			}
			utf8 = temp;
		}

		for (int j = 0; j <= skip && res[j] != '\0'; j++) {
			utf8[count++] = res[j];
		}

		free(res);
	}

	utf8[count] = '\0';
	return utf8;

}
