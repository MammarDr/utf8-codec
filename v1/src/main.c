#include <windows.h>
#include "read_line.h"
#include "utf8.h"



int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    while (1 == 1) {
        int choice;
        printf("1- Encrypt\n2- Decrypt\n3- Exit\n");
        char* inp = read_line(stdin);
        char** result = utf8_decode(inp);
        if (result == NULL) {
            printf("Decode failed");
            return -1;
        }

        for (int i = 0; result[i] != NULL; i++) {
            printf("\n%s\n", result[i]);
            free(result[i]);
        }

        free(result);
        free(inp);
    }
 
}

