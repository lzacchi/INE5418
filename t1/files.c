#include <string.h>

#include "files.h"

void prepend(char *s, const char *t)
{
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

char *read_file(char *filename)
{
    long numbytes;
    FILE *ptr;
    prepend(filename, "public/"); // path to where we keep the served files
    ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        perror("File not found");
    }
    fseek(ptr, 0L, SEEK_END);
    long filesize = ftell(ptr);
    fseek(ptr, 0L, SEEK_SET);
    char *buffer = (char *)calloc(filesize, sizeof(char));
    if (buffer == NULL)
    {
        perror("Calloc error");
        exit(-1);
    }
    fread(buffer, sizeof(char), filesize, ptr);
    fclose(ptr);
    return buffer;
}

void save_file(char *buffer, char *filename)
{
    FILE *ptr;
    ptr = fopen(filename, "w");
    if (ptr == NULL)
    {
        perror("Error when opening file at save_file");
    }
    fprintf(ptr, "%s", buffer);
    fclose(ptr);
}
