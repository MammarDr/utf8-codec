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

int main(void)
{
    test_utf8_char_length();
    test_decode();
    printf("All UTF-8 tests passed.\n");
    return 0;
}
