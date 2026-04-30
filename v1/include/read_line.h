#ifndef READ_LINE_H
#define READ_LINE_H
	#include <stdio.h>
	#include <stdlib.h>

	#ifdef _WIN32
        static char* read_line(FILE* stream)
        {
            size_t capacity = 256;
            size_t length = 0;

            char* buffer = (char*)malloc(sizeof(char) * capacity);
            if (buffer == NULL) {
                free(buffer);
                return NULL;
            }

            int c;

            while ((c = fgetc(stream)) != EOF) {
                
                if (length + 1 >= capacity) {
                    capacity = capacity * 2;
                    char* temp = (char*)realloc(buffer, capacity);
                    if (temp == NULL) {
                        free(buffer);
                        return NULL;
                    }

                    buffer = temp;
                }

                if (c == '\n') break; // removing break line for easier logic
                buffer[length++] = (char)c;

            }

            if (length == 0 && c == EOF) {
                free(buffer);
                return NULL;
            }

            buffer[length] = '\0';

            return buffer;
        }
	#else
	#define _POSIX_C_SOURCE 200809L
	#include <sys/types.h>
        static char* read_line(FILE* stream)
        {
            char* line = NULL;
            size_t capacity = 0;

            ssize_t length = getline(&line, &capacity, stream);

            if (length == -1) {
                free(line);
                return NULL;
            }

            return line;
        }

	#endif
#endif
