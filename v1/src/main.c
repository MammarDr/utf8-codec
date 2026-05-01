#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "read_line.h"
#include "utf8.h"


static void trim(char* s) {
    char* start = s;
    while (isspace((unsigned char)*start)) start++;

    char* end = start + strlen(start);
    while (end > start && isspace((unsigned char)*(end - 1))) end--;

    size_t len = end - start;
    memmove(s, start, len);
    s[len] = '\0';
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    while (1) { 
        printf("\n1- Decode UTF-8 (and re-encode)\n2- Encode to UTF-8\n3- Exit\nChoice: ");
        char* choice_str = read_line(stdin);
        if (!choice_str) break;
        int choice = atoi(choice_str);
        free(choice_str);

        if (choice == 1) {
            printf("\nEnter string to decode: ");
            char* inp = read_line(stdin);
            if (!inp) continue;

            char** decoded = utf8_decode(inp);
            if (decoded == NULL) {
                printf("\nDecode failed\n");
            } else {
                printf("\nDecoded Hex Codepoints:\n");
                int count = 0;
                for (; decoded[count] != NULL; count++) {
                    printf("U+%s ", decoded[count]);
                }
                printf("\n\nEncode the same input ? (y/n)\n");

                choice_str = read_line(stdin);

                if (strcmp(choice_str, "y") == 0 || strcmp(choice_str, "Y") == 0) {
                    uint8_t* re_encoded = utf8_encode(decoded);
                    if (re_encoded) {
                        printf("\n\nEncoded Hex Bytes: \n");
                        for (int i = 0; re_encoded[i] != '\0'; i++) {
                            printf("U+%06X ", (unsigned char)re_encoded[i]);
                        }
                        printf("\nRe-encoded String: %s\n", (char*)re_encoded);
                    }
                }

                

                for (int i = 0; decoded[i] != NULL; i++) {
                    free(decoded[i]);
                }
                free(decoded);
            }
            free(inp);
        }
        else if (choice == 2) {
            printf("Example: 00FE2D, 000041, 01F609 (first zeros can be ignored)\n\n");
            //   "000041", "01F609", "0000E9", "00FE2D", "000041", NULL
          
   


            choice_str = read_line(stdin);
            char** tokens = malloc(sizeof(char*) * (strlen(choice_str) / 2 + 2));
            size_t count = 0;

            char* context = NULL; 


            char* token = strtok_s(choice_str, ",", &context);
            while (token) {
                trim(token);
                tokens[count] = _strdup(token); 
                count++;
                token = strtok_s(NULL, ",", &context);
            }
            tokens[count] = NULL;
            uint8_t* result = utf8_encode(tokens);
            if (result) {
                printf("Encoded Hex Bytes: ");
                for (int i = 0; result[i] != '\0'; i++) {
                    printf("%02X ", (unsigned char)result[i]);
                }
                printf("\nEncoded String: %s\n", (char*)result);
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            printf("Invalid choice.\n");
        }


        printf("\n\nClick to continue.....");
        read_line(stdin);

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }
 
    return 0;
}

