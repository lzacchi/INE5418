#include <string.h>

#include "file_handler.h"

void save_file(char *file_content, char *file)
{
    FILE *file_pointer;
    file_pointer = fopen(file, "w");
    if (file_pointer == NULL)
    {
        perror("Error: Can't save file\n");
    }
    fprintf(file_pointer, "%s", file_content);
    fclose(file_pointer);
}

char *read_file(char *file)
{
    FILE *file_pointer;
    file_pointer = fopen(file, "rb");

    if (file_pointer == NULL)
    {
        perror("Error: No such file\n");
    }

    fseek(file_pointer, 0L, SEEK_END);
    long filesize = ftell(file_pointer);
    fseek(file_pointer, 0L, SEEK_SET);

    char *file_content = (char *)calloc(filesize, sizeof(char));
    if (file_content == NULL)
    {
        perror("file_content error\n");
        exit(1);
    }
    fread(file_content, sizeof(char), filesize, file_pointer);
    fclose(file_pointer);
    return file_content;
}
