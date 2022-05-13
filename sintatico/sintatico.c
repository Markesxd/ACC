#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Token.h"
#include "../sintatico/sintatico.h"

Variable group[100];
int groupSize = 0;

void sintatico(Tokens *list, int numberOfTokens)
{
    int tracker = 0, base = 0;
    while (tracker < numberOfTokens - 1)
    {
        analise(list, &tracker);
    }
    printf("%s = %d\n", group[0].name, group[0].value);
}

int analise(Tokens *list, int *tracker)
{
    if (list[*tracker].type == RESERVED_WORD)
    {
        return filterReservedWord(list, tracker);
    }
    if (list[*tracker].type == IDENTIFIER)
    {
        set(list, tracker);
    }
}

int expression(Tokens *list, int *tracker)
{
    int iValue = 0;
    (*tracker)++;
    if (!strcmp(list[*tracker].token, "(") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT)
    {
        iValue = T(list, tracker);
        iValue += Eline(list, tracker);
        return iValue;
    }
}

int T(Tokens *list, int *tracker)
{
    int iValue = 0;
    if (!strcmp(list[*tracker].token, "(") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT)
    {
        iValue = F(list, tracker);
        iValue *= Tline(list, tracker);
        return iValue;
    }
}

int F(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "("))
    {
        return expression(list, tracker);
    }
    if (list[*tracker].type == IDENTIFIER)
    {
        return 0;
    }
    if (list[*tracker].type == FLOAT)
    {
        return atof(list[*tracker].token);
    }
    if (list[*tracker].type == INT)
    {
        (*tracker)++;
        return atoi(list[*tracker - 1].token);
    }
}

int Tline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "+"))
    {
        return 1;
    }
    if (!strcmp(list[*tracker].token, "*"))
    {
        (*tracker)++;
        int iValue = F(list, tracker);
        iValue *= Tline(list, tracker);
        return iValue;
    }
    if (!strcmp(list[*tracker].token, ")"))
    {
        return 1;
    }
    return 1;
}

int Eline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "+"))
    {   
        (*tracker)++;
        int iValue = T(list, tracker);
        iValue += Eline(list, tracker);
        return iValue;
    }
    if (!strcmp(list[*tracker].token, ")"))
    {
        return 0;
    }
    return 0;
}

int filterReservedWord(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "if"))
    {
        handleIf(list, tracker);
    }
    else if (!strcmp(list[*tracker].token, "int"))
    {
        return handleIntDeclaration(list, tracker);
    }
}

int handleIntDeclaration(Tokens *list, int *tracker)
{
    (*tracker)++;
    if (list[*tracker].type != IDENTIFIER)
    {
        return ID_EXP_ERR;
    }
    (*tracker)++;
    if (list[*tracker].type != SEPARATOR)
    {
        return SEP_EXP_ERR;
    }
    group[groupSize].name = list[(*tracker) - 1].token;
    group[groupSize].type = list[*tracker - 1].type;
    // group[groupSize].value = (int *)malloc(sizeof(int));
    group[groupSize].value = 0;
    groupSize++;
    (*tracker)++;
    return 0;
}

int handleIf(Tokens *list, int *tracker)
{
}

int set(Tokens *list, int *tracker)
{
    int idPlace = *tracker;
    (*tracker)++;
    if (strcmp(list[*tracker].token, "="))
        return error(-1);
    int value = expression(list, tracker);
    for (int i = 0; i < groupSize; i++)
    {
        if (!strcmp(group[i].name, list[idPlace].token))
        {
            group[i].value = value;
            break;
        }
    }
}

int error(int code)
{
}