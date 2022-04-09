#include <stdio.h>
#include "general.h"

int sizeOfFile(FILE *file){
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);
    return size;
}
