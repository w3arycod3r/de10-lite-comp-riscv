#include "str_lib.h"


// Return the length of the null-terminated string
int strlen(char* str) {
    if (str == NULL) {
        return -1;
    }

    int count = 0;
    while (str[count] != '\0') {
        count++;
    }

    return count;
}

// Append src to dest. Dest should be large enough to hold the concatenation.
char *strcat(char *dest, const char *src) {
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    // Save starting point of dest
    char *start_dest = dest;

    // Find the end of dest
    while (*dest != '\0') {
        dest++;
    }

    // Copy to the end of src
    while (*src != '\0') {
        *dest = *src;
        src++;
        dest++;
    }

    // Add null term on dest
    *dest = '\0';

    return start_dest;
}

// Copy src to dest. Dest should be large enough to hold the src string.
// Returns pointer to dest.
char *strcpy(char *dest, const char *src) {
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    // Save starting point of dest
    char *start_dest = dest;

    // Copy to the end of src
    while (*src != '\0') {
        *dest = *src;
        src++;
        dest++;
    }

    // Add null term on dest
    *dest = '\0';

    return start_dest;
}