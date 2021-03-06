#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Token.h"
#include "./lexico/lexico.h"
#include "general.h"
#include "./sintatico/sintatico.h"

int main(int argc, char *argv[]){
    FILE *p;
    if(argc < 2)
    {
        printf("No input file.\n");
        return 1;
    }
    p = fopen(argv[1], "r");
    if(p == NULL)
    {
        printf("File '%s' not found\n", argv[1]);
        return 1;
    }
    int sizeFile = sizeOfFile(p);
    char *code = (char*) malloc(sizeof(char) * sizeFile);
    Tokens *list;
    for(int i = 0; i < sizeFile; i++){
        char c;
        c = fgetc(p);
        code[i] = c;
    }
    int n; 
    list = (Tokens*) lexico(code, &n);
    // printTokens(list, n);
    sintatico(list, n);
    fclose(p);
    return 0;
}