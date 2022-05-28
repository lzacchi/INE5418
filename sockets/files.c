#include <string.h>

#include "files.h"

char *read_file(char *filename)
{
    long numbytes;
    FILE *ptr;
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
        perror("Buffer error");
        exit(1);
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
        perror("Error opening file to save");
    }
    fprintf(ptr, "%s", buffer);
    fclose(ptr);
}
