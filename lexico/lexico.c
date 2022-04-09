#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lexico.h"
#include "Token.h"
#include "../general.h"

int eot(char c)
{
    return (
        c == ' ' ||
        c == '\n' ||
        c == '\t' ||
        c == '+' ||
        c == '-' ||
        c == '/' ||
        c == '*' ||
        c == '<' ||
        c == '>' ||
        c == '=' ||
        c == '(' ||
        c == ')' ||
        c == ',' ||
        c == ';' ||
        c == '}' ||
        c == '{');
}

int eol(char c)
{
    return c == '\n';
}

int isWhiteNoise(char c)
{
    return (
        c == ' ' ||
        c == '\0' ||
        c == '\t');
}

int typify(char *token, char **vocab)
{
    if (isReservedWord(token, vocab))
        return RESERVED_WORD;
    if (isInt(token))
        return INT;
    if (isFloat(token))
        return FLOAT;
    if (isOperator(token))
        return OPERATOR;
    if (isSeparator(token))
        return SEPARATOR;
    if (!strcmp(token, "("))
        return OPEN_PARENTESIS;
    if (!strcmp(token, ")"))
        return CLOSE_PARENTESIS;
    if (!strcmp(token, "{"))
        return OPEN_CURLY;
    if (!strcmp(token, "}"))
        return CLOSE_CURLY;
    if (isValidIdentifier(token))
        return IDENTIFIER;
    return LEXIC_ERR;
}

void getReservedWords(char ***vocab)
{
    FILE *f = fopen("./lexico/reservedWords.txt", "r");
    if (f == 0)
    {
        perror("falha ao ler arquivo");
        exit(1);
    }
    int numberOfWords = 10;
    char *string = malloc(sizeof(char) * 31);
    (*vocab) = malloc(sizeof(char *) * numberOfWords);
    for (int i = 0; i < numberOfWords; i++)
    {
        (*vocab)[i] = (char *)malloc(sizeof(char) * 30);
    }
    for (int i = 0;; i++)
    {
        if (feof(f))
            break;
        fgets((*vocab)[i], 30, f);
        (*vocab)[i][strlen((*vocab)[i]) - 1] = '\0';
    }
    fclose(f);
}

int isReservedWord(char *token, char **vocab)
{
    for (int i = 0; i < 6; i++)
    {
        if (!strcmp(vocab[i], token))
            return 1;
    }

    return 0;
}

int isInt(char *token)
{
    return regex("^[0-9]+$", token);
}

int isFloat(char *token)
{
    return regex("^[0-9]+[.][0-9]+$", token);
}

int isSeparator(char *token)
{
    return !strcmp(token, ";");
}

int isOperator(char *token)
{
    return regex("^[\\<\\>\\=\\+\\-\\*\\/\\%%]$", token);
}

int isValidIdentifier(char *token)
{
    return regex("^([A-Z]|[a-z]|_)([a-z]|[A-Z]|[0-9]|_)*$", token);
}

int regex(char *expression, char *string)
{
    regex_t reg;
    regcomp(&reg, expression, REG_EXTENDED);
    return !regexec(&reg, string, 0, NULL, 0);
}

void printType(int type)
{
    switch (type)
    {
    case INT:
        printf("INT");
        break;
    case RESERVED_WORD:
        printf("R. WORD");
        break;
    case IDENTIFIER:
        printf("IDENTIFIER");
        break;

    case SEPARATOR:
        printf("SEPARATOR");
        break;

    case OPERATOR:
        printf("OPERATOR");
        break;

    case FLOAT:
        printf("FLOAT");
        break;
    case OPEN_PARENTESIS:
        printf("OPEN P.");
        break;
    case CLOSE_PARENTESIS:
        printf("CLOSE P.");
        break;
    case OPEN_CURLY:
        printf("OPEN C.");
        break;
    case CLOSE_CURLY:
        printf("CLOSE C.");
        break;
    default:
        printf("ERROR");
    }
}

void lexico(char *code)
{
    int scout = 0, infantry = 0;
    Tokens *list = (Tokens *)malloc(sizeof(Tokens) * 100);
    int size = strlen(code);
    int numberOfTokens = 0;
    int line = 1, column = 1;
    char **vocab;
    getReservedWords(&vocab);

    while (infantry < size)
    {
        if (isWhiteNoise(code[infantry]))
        {
            infantry++;
            scout++;
            continue;
        }
        if (eol(code[infantry]))
        {
            column = 1;
            line++;
            infantry++;
            scout++;
            continue;
        }
        while (!eot(code[scout++]) && scout <= size)
            ;

        int n = scout - infantry - 1;
        if (n)
        {
            list[numberOfTokens].column = column;
            list[numberOfTokens].line = line;
            strncpy(list[numberOfTokens].token, code + infantry, scout - infantry - 1);
            list[numberOfTokens].type = typify(list[numberOfTokens].token, vocab);
            numberOfTokens++;
        }
        column += scout - infantry;
        infantry = scout;

        if (!isWhiteNoise(code[scout - 1]))
        {
            list[numberOfTokens].column = column - 1;
            list[numberOfTokens].line = line;
            list[numberOfTokens].token[0] = code[scout - 1];
            list[numberOfTokens].token[1] = '\0';
            char aux[2];
            aux[0] = code[scout - 1];
            aux[1] = '\0';
            list[numberOfTokens].type = typify(aux, vocab);
            numberOfTokens++;
        }
    }

    for (int i = 0; i < numberOfTokens; i++)
    {
        printf("(%d x %d) ", list[i].line, list[i].column);
        printType(list[i].type);
        printf(" ");
        puts(list[i].token);
    }
    printf("\n");
}