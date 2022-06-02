#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Token.h"
#include "../sintatico/sintatico.h"

Variable group[100];
int groupSize = 0;

void sintatico(Tokens *list, int numberOfTokens)
{
    int tracker = 0;

    code(list, &tracker);

    for (int i = 0; i < groupSize; i++)
        printf("'%s' declared\n", group[i].name);
}

int code(Tokens *list, int *tracker)
{
    if (list[*tracker].type == EOF)
        return 0;
    command(list, tracker);
    code(list, tracker);
}

int command(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "int") || !strcmp(list[*tracker].token, "float"))
    {
        (*tracker)++;
        return declaration(list, tracker);
    }
    if (list[*tracker].type == IDENTIFIER)
    {
        return set(list, tracker);
    }
    if (!strcmp(list[*tracker].token, "if"))
    {
        (*tracker)++;
        return condition(list, tracker);
    }
    if(!strcmp(list[*tracker].token, 'while'))

    error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    (*tracker)++;
    return -1;
}

int expression(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT)
    {
        E(list, tracker);
        Xline(list, tracker);
        return 0;
    }

    error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    (*tracker)++;
    return -1;
}

int Xline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, ">") ||
        !strcmp(list[*tracker].token, "<"))
    {
        (*tracker)++;
        E(list, tracker);
        Xline(list, tracker);
        return 0;
    }
    return 0;
}

int E(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT)
    {
        T(list, tracker);
        Eline(list, tracker);
        return 0;
    }
    error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    return -1;
}

int T(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT)
    {
        int ret = F(list, tracker);
        if(ret < 0)
            return ret;
        Tline(list, tracker);
        return 0;
    }
    error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    return -1;
}

int F(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "("))
    {
        (*tracker)++;
        expression(list, tracker);
        if (!strcmp(list[*tracker].token, ")"))
        {
            (*tracker)++;
            return 0;
        }
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return PAR_EXP_ERR;
    }
    if (list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == INT ||
        list[*tracker].type == FLOAT)
    {
        // for (int i = 0; i < groupSize; i++)
        // {
        //     (*tracker)++;
        //     if (!strcmp(group[i].name, list[*tracker - 1].token))
        //         return group[i].value;
        // }
        (*tracker)++;
        return 0;
    }
    error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    return -1;
}

int Tline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "+") || !strcmp(list[*tracker].token, "-"))
    {
        return 0;
    }
    if (!strcmp(list[*tracker].token, "*"))
    {
        (*tracker)++;
        F(list, tracker);
        Tline(list, tracker);
        return 0;
    }
    if (!strcmp(list[*tracker].token, "/"))
    {
        (*tracker)++;
        F(list, tracker);
        Tline(list, tracker);
        return 0;
    }
    if (!strcmp(list[*tracker].token, ")"))
    {
        return 0;
    }
    return 0;
}

int Eline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "+"))
    {
        (*tracker)++;
        T(list, tracker);
        Eline(list, tracker);
        return 0;
    }
    if (!strcmp(list[*tracker].token, "-"))
    {
        (*tracker)++;
        T(list, tracker);
        Eline(list, tracker);
        return 0;
    }
    if (!strcmp(list[*tracker].token, ")"))
    {
        return 0;
    }
    return 0;
}

int declaration(Tokens *list, int *tracker)
{
    if (list[*tracker].type != IDENTIFIER)
    {
        error(ID_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return ID_EXP_ERR;
    }
    (*tracker)++;

    if (list[*tracker].type != SEPARATOR)
    {
        error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return SEP_EXP_ERR;
    }
    group[groupSize].name = list[(*tracker) - 1].token;
    group[groupSize].type = list[*tracker - 1].type;
    group[groupSize].value = 0;
    groupSize++;
    (*tracker)++;
    return 0;
}

int condition(Tokens *list, int *tracker)
{
    if (strcmp(list[*tracker].token, "("))
    {
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return PAR_EXP_ERR;
    }
    (*tracker)++;

    expression(list, tracker);

    if (strcmp(list[(*tracker)].token, ")"))
    {
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return PAR_EXP_ERR;
    }
    (*tracker)++;

    command(list, tracker);
}

int set(Tokens *list, int *tracker)
{
    // int idPlace = *tracker;
    (*tracker)++;

    if (strcmp(list[*tracker].token, "="))
    {
        error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return OP_EXP_ERR;
    }
    (*tracker)++;

    expression(list, tracker);

    if (list[*tracker].type != SEPARATOR)
    {
        error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return SEP_EXP_ERR;
    }

    (*tracker)++;
    return 0;
}

int error(int code, int line, int column)
{
    printf("%d:%d: Sintatic error: ", line, column);
    switch (code)
    {
    case ID_EXP_ERR:
        printf("Identifier expected\n");
        break;
    case SEP_EXP_ERR:
        printf("';' expected\n");
        break;
    case PAR_EXP_ERR:
        printf("Parentesis missing\n");
        break;
    case OP_EXP_ERR:
        printf("Operator expected\n");
        break;
    case UNE_TKN_ERR:
        printf("Unexpected Token\n");
        break;
    }
}