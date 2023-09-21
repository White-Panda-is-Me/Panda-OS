#include "string.h"

int strlen(char* str) {
    int i = 0;
    while(*str) {
        i++;
        str++;
    }
    return i;
}

int strncmp(char* str1, char* str2, int size) {
    for (int i = 0;i < size;i++) {
        if(str1[i] != str2[i])
            return 1;
    }
    return 0;
}

int strcmp(char* str1, char* str2) {
    if(strlen(str1) != strlen(str2))
        return 1;
    while (*str1) {
        if(*str1 != *str2)
            return 1;
        str1++;
        str2++;
    }
    return 0;
}

char outStr[124];
char* substr(char* str, int i1, int i2) {
    for (int i = 0;i < 124;i++) {
        outStr[i] = 0;
    }
    int len = i2 - i1;
    str += i1;
    for (int i = 0;i < (len + 1);i++) {
        outStr[i] = *str;
        str++;
        if(i == len)
            outStr[i+1] = '\0';
    }
    return outStr;
}