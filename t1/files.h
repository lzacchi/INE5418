#pragma once

#include <stdlib.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

void prepend(char *s, const char *t);
char *read_file(char *filename);
void save_file(char *buffer, char *filename);
