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
        analyse(list, &tracker);
    }
    for(int i = 0; i < groupSize; i++)
        printf("%s = %.4f\n", group[i].name, group[i].value);
}

int analyse(Tokens *list, int *tracker)
{
    if (list[*tracker].type == LEXIC_ERR) (*tracker)++;
    if (list[*tracker].type == RESERVED_WORD)
    {
        return filterReservedWord(list, tracker);
    }
    if (list[*tracker].type == IDENTIFIER)
    {
        set(list, tracker);
    }
}

float expression(Tokens *list, int *tracker)
{
    float value = 0;
    (*tracker)++;
    if (!strcmp(list[*tracker].token, "(") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT)
    {
        value = T(list, tracker);
        value += Eline(list, tracker);
        return value;
    }
}

float T(Tokens *list, int *tracker)
{
    float value = 0;
    if (!strcmp(list[*tracker].token, "(") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT)
    {
        value = F(list, tracker);
        value *= Tline(list, tracker);
        return value;
    }
}

float F(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "("))
    {
        return expression(list, tracker);
    }
    if (list[*tracker].type == IDENTIFIER)
    {
        for(int i = 0; i < groupSize; i++)
        {
            (*tracker)++;
            if(!strcmp(group[i].name, list[*tracker - 1].token))
                return group[i].value;
        }
    }
    if (list[*tracker].type == FLOAT)
    {
        (*tracker)++;
        return atof(list[*tracker - 1].token);
    }
    if (list[*tracker].type == INT)
    {
        (*tracker)++;
        return atoi(list[*tracker - 1].token);
    }
}

float Tline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "+"))
    {
        return 1;
    }
    if (!strcmp(list[*tracker].token, "*"))
    {
        (*tracker)++;
        float value = F(list, tracker);
        value *= Tline(list, tracker);
        return value;
    }
    if (!strcmp(list[*tracker].token, ")"))
    {
        (*tracker)++;
        return 1;
    }
    return 1;
}

float Eline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "+"))
    {
        (*tracker)++;
        float value = T(list, tracker);
        value += Eline(list, tracker);
        return value;
    }
    if (!strcmp(list[*tracker].token, ")"))
    {
        (*tracker)++;
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
    group[groupSize].value = 0;
    groupSize++;
    (*tracker)++;
    return 0;
}

int handleIf(Tokens *list, int *tracker)
{
    (*tracker)++;
    if (!strcmp(list[*tracker].token, "("))
    {
        error(PAR_EXP_ERR);
    }
    float value = expression(list, tracker);
    if(strcmp(list[(*tracker) - 1].token, ")"))
    {
        error(PAR_EXP_ERR);
    }
    if(!value){
        while(list[*tracker].type != SEPARATOR) (*tracker)++;
    }
}

int set(Tokens *list, int *tracker)
{
    int idPlace = *tracker;
    (*tracker)++;
    if (strcmp(list[*tracker].token, "="))
        return error(-1);
    float value = expression(list, tracker);
    for (int i = 0; i < groupSize; i++)
    {
        if (!strcmp(group[i].name, list[idPlace].token))
        {
            group[i].value = value;
            break;
        }
    }
    if (list[*tracker].type != SEPARATOR)
    {
        error(SEP_EXP_ERR);
    }
    (*tracker)++;
}

int error(int code)
{
}