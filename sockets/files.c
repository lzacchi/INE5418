#include "files.h"

char *read_file(char *filename, long *filesize)
{
    long numbytes;
    FILE *ptr;
    ptr = fopen(filename, "r");

    if (ptr == NULL)
    {
        perror("File not found");
    }

    fseek(ptr, 0L, SEEK_END);
    long tmp = ftell(ptr);
    filesize = &tmp;
    fseek(ptr, 0L, SEEK_SET);

    char *buffer = (char *)calloc(*filesize, sizeof(char));
    if (buffer == NULL)
    {
        perror("Buffer error");
        exit(1);
    }
    fread(buffer, sizeof(char), *filesize, ptr);
    fclose(ptr);
    return buffer;
}
