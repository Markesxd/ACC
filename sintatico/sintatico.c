#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Token.h"
#include "../sintatico/sintatico.h"

Variable variables[100];
int numberOfVariables = 0;

void sintatico(Tokens *list, int numberOfTokens)
{
    int tracker = 0;
    code(list, &tracker);
    checkIfUsed();
    for (int i = 0; i < numberOfVariables; i++)
    {
        printf("%s = %.2f\n", variables[i].name, variables[i].value);
    }
}

int code(Tokens *list, int *tracker)
{
    if (list[*tracker].type == EOF)
        return 0;
    command(list, tracker, 0);
    code(list, tracker);
}

int command(Tokens *list, int *tracker, int inLoop)
{
    if (!strcmp(list[*tracker].token, "int") || !strcmp(list[*tracker].token, "float") || !strcmp(list[*tracker].token, "char"))
    {
        (*tracker)++;
        return declaration(list, tracker);
    }
    // if (!strcmp(list[*tracker].token, "char"))
    // {
    //     (*tracker)++;
    //     return charDeclaration(list, tracker);
    // }
    if (!strcmp(list[*tracker].token, "+") ||
        !strcmp(list[*tracker].token, "-") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == LEXIC_ERR)
    {
        return set(list, tracker, 0);
    }
    if (!strcmp(list[*tracker].token, "if"))
    {
        (*tracker)++;
        return condition(list, tracker, 0, inLoop);
    }
    if (!strcmp(list[*tracker].token, "while"))
    {
        (*tracker)++;
        return condition(list, tracker, 1, 1);
    }
    if (!strcmp(list[*tracker].token, "for"))
    {
        (*tracker)++;
        return loop(list, tracker);
    }
    if (!strcmp(list[*tracker].token, "return"))
    {
        (*tracker)++;

        if (list[*tracker].type != SEPARATOR)
        {
            expression(list, tracker);
        }

        if (list[*tracker].type != SEPARATOR)
        {
            error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;
        return 0;
    }
    if (
        !strcmp(list[*tracker].token, "break") ||
        !strcmp(list[*tracker].token, "continue"))
    {
        if (!inLoop)
        {
            error(LP_ONL_ERR, list[*tracker].line, list[*tracker].column);
        }

        (*tracker)++;

        if (list[*tracker].type != SEPARATOR)
        {
            error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;
        return 0;
    }
    if (list[*tracker].type == SEPARATOR)
    {
        (*tracker)++;
        return 0;
    }
    if (list[*tracker].type == EOF)
    {
        error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
        return -1;
    }
    error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    (*tracker)++;
    return -1;
}

double expression(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        double value = 0;
        value = X2(list, tracker, value);
        value = Xline(list, tracker, value);
        return value;
    }
    if (!strcmp(list[*tracker].token, ">") ||
        !strcmp(list[*tracker].token, "<") ||
        !strcmp(list[*tracker].token, ",") ||
        !strcmp(list[*tracker].token, "=") ||
        !strcmp(list[*tracker].token, "&") ||
        !strcmp(list[*tracker].token, "*") ||
        !strcmp(list[*tracker].token, "+") ||
        !strcmp(list[*tracker].token, "/") ||
        list[*tracker].token[0] == '%' ||
        !strcmp(list[*tracker].token, "|"))
    {
        error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)++;
        X2(list, tracker, 0);
        Xline(list, tracker, 0);
        return -1;
    }
    error(EX_EXP_ERR, list[*tracker].line, list[*tracker].column);
    // (*tracker)++;
    return -1;
}

double X2line(Tokens *list, int *tracker, double prevValue)
{
    if (!strcmp(list[*tracker].token, "<"))
    {
        (*tracker)++;
        if (!strcmp(list[*tracker].token, "="))
        {
            (*tracker)++;
            double value = E(list, tracker);
            value = prevValue <= value;
            value = X2line(list, tracker, value);
            return value;
        }
        double value = E(list, tracker);
        value = prevValue < value;
        value = X2line(list, tracker, value);
        return value;
    }
    if (!strcmp(list[*tracker].token, ">"))
    {
        (*tracker)++;
        if (!strcmp(list[*tracker].token, "="))
        {
            (*tracker)++;
            double value = E(list, tracker);
            value = prevValue >= value;
            value = X2line(list, tracker, value);
            return value;
        }
        double value = E(list, tracker);
        value = prevValue > value;
        value = X2line(list, tracker, value);
        return value;
    }
    if (!strcmp(list[*tracker].token, "!"))
    {
        (*tracker)++;
        if (strcmp(list[*tracker].token, "="))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;
        double value = E(list, tracker);
        value = prevValue != value;
        value = X2line(list, tracker, value);
        return value;
    }
    if (!strcmp(list[*tracker].token, "="))
    {
        (*tracker)++;
        if (strcmp(list[*tracker].token, "="))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;
        double value = E(list, tracker);
        value = prevValue == value;
        value = X2line(list, tracker, value);
        return value;
    }

    return prevValue;
}

double X2(Tokens *list, int *tracker, double prevValue)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        double value;
        value = E(list, tracker);
        value = X2line(list, tracker, value);
        return value;
    }
    return prevValue;
}

double Xline(Tokens *list, int *tracker, double prevValue)
{
    if (!strcmp(list[*tracker].token, "&"))
    {
        (*tracker)++;
        if (strcmp(list[*tracker].token, "&"))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;

        double value = X2(list, tracker, prevValue);
        value = prevValue && value;
        value = Xline(list, tracker, value);
        return value;
    }
    if (!strcmp(list[*tracker].token, "|"))
    {
        (*tracker)++;
        if (strcmp(list[*tracker].token, "|"))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;
        double value = X2(list, tracker, prevValue);
        value = prevValue || value;
        value = Xline(list, tracker, value);
        return value;
    }

    return prevValue;
}

double E(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        double value;
        value = T(list, tracker);
        value += Eline(list, tracker);
        return value;
    }
    return -1;
}

double T(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        double value = F(list, tracker);
        value *= Tline(list, tracker, value);
        return value;
    }
    error(TER_EXP_ERR, list[*tracker].line, list[*tracker].column);
    Tline(list, tracker, -1);
    return -1;
}

double F(Tokens *list, int *tracker)
{
    int inverted = 0, denied = 0;
    if (!strcmp(list[*tracker].token, "-"))
    {
        (*tracker)++;
        inverted = 1;
    }
    else if (!strcmp(list[*tracker].token, "!"))
    {
        (*tracker)++;
        denied = 1;
    }
    if (!strcmp(list[*tracker].token, "("))
    {
        (*tracker)++;
        double value = expression(list, tracker);
        if (!strcmp(list[*tracker].token, ")"))
        {
            (*tracker)++;
            return value;
        }
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        if (denied)
            return !value;
        if (inverted)
            return -value;
        return value;
    }
    if (list[*tracker].type == INT ||
        list[*tracker].type == FLOAT)
    {
        double value = atof(list[*tracker].token);
        (*tracker)++;
        if (denied)
            return !value;
        if (inverted)
            return -value;
        return value;
    }
    if (list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == LEXIC_ERR)
    {
        if (!isDeclared(list[*tracker]))
        {
            error(NOT_DEC_ERR, list[*tracker].line, list[*tracker].column);
        }
        makeUsed(list[*tracker].token);
        double value = getValue(list[*tracker].token);
        (*tracker)++;
        if (denied)
            return !value;
        if (inverted)
            return -value;
        return value;
    }
    if (list[*tracker].type == EOF)
    {
        return -1;
    }
    error(TER_EXP_ERR, list[*tracker].line, list[*tracker].column);
    // (*tracker)++;
    return -1;
}

double Tline(Tokens *list, int *tracker, int prevValue)
{
    if (!strcmp(list[*tracker].token, "+") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, ">") ||
        !strcmp(list[*tracker].token, "<") ||
        !strcmp(list[*tracker].token, "!") ||
        !strcmp(list[*tracker].token, ",") ||
        !strcmp(list[*tracker].token, "=") ||
        !strcmp(list[*tracker].token, "&") ||
        !strcmp(list[*tracker].token, "|") ||
        !strcmp(list[*tracker].token, ";"))
    {
        return 1;
    }
    if (!strcmp(list[*tracker].token, "*"))
    {
        (*tracker)++;
        double value = F(list, tracker);
        value *= Tline(list, tracker, value);
        return value;
    }
    if (!strcmp(list[*tracker].token, "/"))
    {
        (*tracker)++;
        double value = F(list, tracker);
        value *= Tline(list, tracker, value);
        return 1 / value;
    }
    if (list[*tracker].token[0] == '%')
    {
        (*tracker)++;
        double value = F(list, tracker);
        value *= Tline(list, tracker, value);
        return (double)(prevValue % (int)value) / prevValue;
    }
    if (!strcmp(list[*tracker].token, ")"))
    {
        return 1;
    }
    if (list[*tracker].type == EOF)
    {
        return -1;
    }
    if (list[*tracker].type == INT ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == LEXIC_ERR)
    {
        error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        F(list, tracker);
        Tline(list, tracker, -1);
        return -1;
    }
    // error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    return UNE_TKN_ERR;
}

double Eline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "+"))
    {
        (*tracker)++;

        double value = T(list, tracker);
        value += Eline(list, tracker);
        return value;
    }
    if (!strcmp(list[*tracker].token, "-"))
    {
        (*tracker)++;

        double value = T(list, tracker);
        value += Eline(list, tracker);
        return -value;
    }
    if (!strcmp(list[*tracker].token, ")") ||
        !strcmp(list[*tracker].token, ">") ||
        !strcmp(list[*tracker].token, "<") ||
        !strcmp(list[*tracker].token, "!") ||
        !strcmp(list[*tracker].token, ",") ||
        !strcmp(list[*tracker].token, "=") ||
        !strcmp(list[*tracker].token, "&") ||
        !strcmp(list[*tracker].token, "|") ||
        !strcmp(list[*tracker].token, ";"))
    {
        return 0;
    }
    if (list[*tracker].type == EOF)
    {
        // error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return -1;
    }
    error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
    T(list, tracker);
    Eline(list, tracker);
    return -1;
}

int declaration(Tokens *list, int *tracker)
{
    if (list[*tracker].type != IDENTIFIER && list[*tracker].type != LEXIC_ERR)
    {
        error(ID_EXP_ERR, list[*tracker].line, list[*tracker].column);
        // (*tracker)--;
    }
    int idPlace = *tracker;
    if (isDeclared(list[idPlace]))
    {
        error(RE_DEC_ERR, list[idPlace].line, list[idPlace].column);
    }
    strcpy(variables[numberOfVariables].name, list[idPlace].token);
    variables[numberOfVariables].type = list[idPlace].type;
    variables[numberOfVariables].line = list[idPlace].line;
    variables[numberOfVariables].column = list[idPlace].column;
    variables[numberOfVariables].value = 0;
    numberOfVariables++;
    (*tracker)++;

    A(list, tracker, idPlace);

    if (list[*tracker].type != SEPARATOR)
    {
        error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    (*tracker)++;
    return 0;
}

int A(Tokens *list, int *tracker, int idPlace)
{
    if (!strcmp(list[*tracker].token, "="))
    {
        (*tracker)++;
        double value;
        if (list[*tracker].type == CHAR)
            (*tracker)++;
        else
            value = expression(list, tracker);

        variables[numberOfVariables - 1].isInitialized = 1;
        variables[numberOfVariables - 1].value = value;

        Aline(list, tracker);
        return 0;
    }
    if (!strcmp(list[*tracker].token, ","))
    {
        Aline(list, tracker);
    }
    return 0;
}

int Aline(Tokens *list, int *tracker)
{
    int idPlace;
    if (!strcmp(list[*tracker].token, ","))
    {
        (*tracker)++;

        if (list[*tracker].type != IDENTIFIER)
        {
            error(ID_EXP_ERR, list[*tracker].line, list[*tracker].column);
            // (*tracker)--;
        }
        idPlace = *tracker;
        strcpy(variables[numberOfVariables].name, list[idPlace].token);
        variables[numberOfVariables].type = list[idPlace].type;
        variables[numberOfVariables].line = list[idPlace].line;
        variables[numberOfVariables].column = list[idPlace].column;
        numberOfVariables++;
        (*tracker)++;
    }

    A(list, tracker, idPlace);

    return 0;
}

int condition(Tokens *list, int *tracker, int loop, int inLoop)
{
    if (strcmp(list[*tracker].token, "("))
    {
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    (*tracker)++;

    expression(list, tracker);

    if (strcmp(list[(*tracker)].token, ")"))
    {
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    (*tracker)++;

    block(list, tracker, inLoop);

    if (!loop && !strcmp(list[*tracker].token, "else"))
    {
        (*tracker)++;
        block(list, tracker, inLoop);
    }
}

int block(Tokens *list, int *tracker, int inLoop)
{
    if (!strcmp(list[*tracker].token, "{"))
    {
        (*tracker)++;
        if (!strcmp(list[*tracker].token, "}"))
        {
            (*tracker)++;
            return 0;
        }
        command(list, tracker, inLoop);
        bline(list, tracker, inLoop);
        return 0;
    }

    command(list, tracker, inLoop);
    return 0;
}

int bline(Tokens *list, int *tracker, int inLoop)
{
    if (list[*tracker].type == EOF)
    {
        error(CUR_EXP_ERR, list[*tracker - 1].line, list[*tracker - 1].column);
        return CUR_EXP_ERR;
    }
    if (!strcmp(list[*tracker].token, "}"))
    {
        (*tracker)++;
        return 0;
    }
    command(list, tracker, inLoop);
    bline(list, tracker, inLoop);
}

int set(Tokens *list, int *tracker, int inFor)
{
    if (!strcmp(list[*tracker].token, "+"))
    {
        (*tracker)++;
        if (strcmp(list[*tracker].token, "+"))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;

        if (list[*tracker].type != IDENTIFIER)
        {
            error(ID_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        if (!isDeclared(list[*tracker]))
        {
            error(NOT_DEC_ERR, list[*tracker].line, list[*tracker].column);
        }
        makeUsed(list[*tracker].token);
        (*tracker)++;

        if (!inFor)
        {
            if (list[*tracker].type != SEPARATOR)
            {
                error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
                (*tracker)--;
            }

            (*tracker)++;
        }
        return 0;
    }

    if (!strcmp(list[*tracker].token, "-"))
    {
        (*tracker)++;
        if (strcmp(list[*tracker].token, "-"))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;

        if (list[*tracker].type != IDENTIFIER)
        {
            error(ID_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        if (!isDeclared(list[*tracker]))
        {
            error(NOT_DEC_ERR, list[*tracker].line, list[*tracker].column);
        }
        makeUsed(list[*tracker].token);
        (*tracker)++;

        if (!inFor)
        {
            if (list[*tracker].type != SEPARATOR)
            {
                error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
                (*tracker)--;
            }

            (*tracker)++;
        }
        return 0;
    }

    if (!isDeclared(list[*tracker]))
    {
        error(NOT_DEC_ERR, list[*tracker].line, list[*tracker].column);
    }
    makeUsed(list[*tracker].token);
    int idPlace = *tracker;
    (*tracker)++;

    if (!strcmp(list[*tracker].token, "+"))
    {
        (*tracker)++;

        if (strcmp(list[*tracker].token, "+"))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;

        if (!inFor)
        {
            if (list[*tracker].type != SEPARATOR)
            {
                error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
                (*tracker)--;
            }

            (*tracker)++;
        }
        return 0;
    }

    if (!strcmp(list[*tracker].token, "-"))
    {
        (*tracker)++;

        if (strcmp(list[*tracker].token, "-"))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;

        if (!inFor)
        {
            if (list[*tracker].type != SEPARATOR)
            {
                error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
                (*tracker)--;
            }

            (*tracker)++;
        }
        return 0;
    }

    if (strcmp(list[*tracker].token, "="))
    {
        error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        return OP_EXP_ERR;
    }
    (*tracker)++;

    double value;
    if (list[*tracker].type == CHAR)
    {
        value = list[*tracker].token[0];
        (*tracker)++;
    }
    else
    {
        value = expression(list, tracker);
    }

    if (!inFor)
    {
        if (list[*tracker].type != SEPARATOR)
        {
            error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }

        (*tracker)++;
    }

    makeInitialized(list[idPlace].token);
    int varPlace = getVarPlace(list[idPlace].token);
    variables[varPlace].value = value;
    return 0;
}

int loop(Tokens *list, int *tracker)
{
    if (strcmp(list[*tracker].token, "("))
    {
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    (*tracker)++;

    if (!strcmp(list[*tracker].token, "int") || !strcmp(list[*tracker].token, "float"))
    {
        (*tracker)++;
        declaration(list, tracker);
    }
    else if (list[*tracker].type == IDENTIFIER || list[*tracker].type == LEXIC_ERR)
    {
        set(list, tracker, 0);
    }

    expression(list, tracker);

    if (list[*tracker].type != SEPARATOR)
    {
        error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    (*tracker)++;

    set(list, tracker, 1);

    if (strcmp(list[*tracker].token, ")"))
    {
        error(PAR_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    (*tracker)++;

    block(list, tracker, 1);
}

int error(int code, int line, int column)
{
    if (line == -1)
        printf("$: ");
    else
        printf("%d:%d: ", line, column);

    if (code > -9)
        printf("Sintatic error: ");
    else
        printf("Semantic error: ");

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
        printf("Valid operator expected\n");
        break;
    case UNE_TKN_ERR:
        printf("Unexpected Token\n");
        break;
    case CUR_EXP_ERR:
        printf("Curly bracess missing\n");
        break;
    case EX_EXP_ERR:
        printf("Expression expected\n");
        break;
    case TER_EXP_ERR:
        printf("Term expected\n");
        break;
    case LP_ONL_ERR:
        printf("Argument is restricted to a loop\n");
        break;
    case NOT_DEC_ERR:
        printf("Variable not declared\n");
        break;
    case RE_DEC_ERR:
        printf("Redeclaration of Variable\n");
        break;
    case NOT_USD_ERR:
        printf("Variable not used\n");
        break;
    }
}

int isDeclared(Tokens token)
{
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (!strcmp(variables[i].name, token.token))
            return 1;
    }
    return 0;
}

void checkIfUsed()
{
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (!variables[i].isUsed)
            error(NOT_USD_ERR, variables[i].line, variables[i].column);
    }
}

void makeUsed(char *variable)
{
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (!strcmp(variable, variables[i].name))
        {
            variables[i].isUsed = 1;
            break;
        }
    }
}

double getValue(char *variable)
{
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (!strcmp(variables[i].name, variable))
        {
            return variables[i].value;
        }
    }
}

int getVarPlace(char *variable)
{
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (!strcmp(variable, variables[i].name))
            return i;
    }
}

void makeInitialized(char *variable)
{
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (!strcasecmp(variable, variables[i].name))
        {
            variables[i].isInitialized = 1;
            return;
        }
    }
}