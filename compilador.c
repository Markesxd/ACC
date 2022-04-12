#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./lexico/Token.h"
#include "./lexico/lexico.h"
#include "general.h"

int main(int argc, char *argv[]){
    FILE *p;
    if(argc < 2)return 1;
    p = fopen(argv[1], "r");
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
    printError(list, n);
    fclose(p);
    return 0;
}

//legaxi
//  if(feof(p)) break;
//     column++;
//     if(eot(c)){
//         if(token[0] == '\0') {
//             cColumn = column;
//             continue;
//         }
//         list[numberOfTokens].column = cColumn;
//         list[numberOfTokens].line = line;
//         list[numberOfTokens].type = 1;
//         strcpy(list[numberOfTokens].token, token);
//         if(eol(c)) line++;
//         numberOfTokens++;
//         sizeOfToken = 0;
//         token[0] = '\0';
//         cColumn = column;
//         continue;
//     }
//     token[sizeOfToken++] = c;
//     token[sizeOfToken] = '\0';