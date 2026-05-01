#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "utf8.h"


void test_utf8_char_length(void)
{
    assert(utf8_char_length(0x41) == 1); // A
    assert(utf8_char_length(0xC3) == 2); // é
    assert(utf8_char_length(0xE2) == 3); // €
    assert(utf8_char_length(0xF0) == 4); // 😉
}

void test_decode(void)
{
    char** result;

    result = utf8_decode("A");
    assert(strcmp(result[0], "00041") == 0);

    result = utf8_decode("\xC3\xA9"); // é
    assert(strcmp(result[0], "000E9") == 0);

    result = utf8_decode("\xE2\x82\xAC"); // €
    assert(strcmp(result[0], "020AC") == 0); 

    result = utf8_decode("\xF0\x9F\x98\x89"); // 😉;
    assert(strcmp(result[0], "1F609") == 0);

    result = utf8_decode("\x41\xD8\xB3\xC3\xA9\xF0\x9F\x98\x89"); // Aسé😉
    assert(strcmp(result[0], "00041") == 0); // A
    assert(strcmp(result[1], "00633") == 0); // س
    assert(strcmp(result[2], "000E9") == 0); // é
    assert(strcmp(result[3], "1F609") == 0); // 😉
 
}

void test_encode(void)
{
    uint8_t* result;
    const char* str1[] = { "00041", NULL };
    result = utf8_encode((char**)str1);
    assert(result[0] == 0x41);
    assert(result[1] == 0x00);
    free(result);

    const char* str2[] = { "000E9", NULL }; // é
    result = utf8_encode((char**)str2);
    assert(result[0] == 0xC3);
    assert(result[1] == 0xA9);
    assert(result[2] == 0x00);
    free(result);

    const char* str3[] = { "020AC", NULL }; // €
    result = utf8_encode((char**)str3);
    assert(result[0] == 0xE2);
    assert(result[1] == 0x82);
    assert(result[2] == 0xAC);
    assert(result[3] == 0x00);
    free(result);

    const char* str4[] = { "1F609", NULL }; // 😉
    result = utf8_encode((char**)str4);
    assert(result[0] == 0xF0);
    assert(result[1] == 0x9F);
    assert(result[2] == 0x98);
    assert(result[3] == 0x89);
    assert(result[4] == 0x00);
    free(result);

    const char* str5[] = { "00041", "00633", "000E9", "1F609", NULL }; // Aسé😉
    result = utf8_encode((char**)str5);
    assert(result[0] == 0x41);
    assert(result[1] == 0xD8);
    assert(result[2] == 0xB3);
    assert(result[3] == 0xC3);
    assert(result[4] == 0xA9);
    assert(result[5] == 0xF0);
    assert(result[6] == 0x9F);
    assert(result[7] == 0x98);
    assert(result[8] == 0x89);
    assert(result[9] == 0x00);
    free(result);
 
}

int main(void)
{
    test_utf8_char_length();
    test_decode();
    test_encode();
    printf("All UTF-8 tests passed.\n");
    return 0;
}
