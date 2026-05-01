#ifndef UTF8_H
#define UTF8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int utf8_char_length(uint8_t lead);
char** utf8_decode(char* str);
uint8_t* utf8_encode(char** unicode);

#endif
