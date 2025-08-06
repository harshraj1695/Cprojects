#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* reverse(const char in[], int length) {
    char* reversed_str = (char*)malloc(sizeof(char) * (length + 1));
    if (reversed_str == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    int i = length - 1;
    int j = 0;

    while (i >= 0) {
        reversed_str[j] = in[i];
        i--;
        j++;
    }

    reversed_str[j] = '\0';

    return reversed_str;
}

int main() {
    char in[] = "hii i ma harsh";
    int len = strlen(in);

    char* reversed_string = reverse(in, len);

    if (reversed_string != NULL) {
        printf("%s\n", reversed_string);
        free(reversed_string);
        reversed_string = NULL;
    } else {
        printf("String reversal failed.\n");
    }

    return 0;
}

