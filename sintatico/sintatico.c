#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Token.h"
#include "../sintatico/sintatico.h"

Variable variables[100];
int numberOfVariables = 0;
int hasError = 0;
int ifNum = 0;
int whileNum = 0;

char *text;
int textPtr = 0;
int textSize = 100;

char *data;
int dataPtr = 0;
int dataSize = 100;

void sintatico(Tokens *list, int numberOfTokens)
{
    text = (char *)malloc(textSize * sizeof(char));
    writeText(".text\n");
    int tracker = 0;

    code(list, &tracker);

    checkIfUsed();
    if (hasError)
    {
        return;
    }
    data = (char *)malloc(dataSize * sizeof(char));
    writeData(".data\n");
    for (int i = 0; i < numberOfVariables; i++)
    {
        writeData(variables[i].name);
        writeData(": ");
        if (variables[i].type == 2)
            writeData(".byte 0\n");
        else
            writeData(".word 0\n");
    }
    writeData("\0");
    writeText("\0");
    puts(data);
    puts(text);
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (variables[i].type == 2)
            printf("(%d) %s = %c\n", variables[i].type, variables[i].name, (int)variables[i].value);
        else
            printf("(%d) %s = %.2f\n", variables[i].type, variables[i].name, variables[i].value);
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
    if (!strcmp(list[*tracker].token, "int"))
    {
        (*tracker)++;
        return declaration(list, tracker, 0);
    }
    if (!strcmp(list[*tracker].token, "float"))
    {
        (*tracker)++;
        return declaration(list, tracker, 1);
    }
    if (!strcmp(list[*tracker].token, "char"))
    {
        (*tracker)++;
        return declaration(list, tracker, 2);
    }
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

        if(!strcmp(list[*tracker].token, "break"))
        {
            zeroT0();
            char label[20];
            sprintf(label, "endwhile%d\n", whileNum - 1);
            jumpEqualsT0(label);
        } else {
            zeroT0();
            char label[20];
            sprintf(label, "while%d\n", whileNum - 1);
            jumpEqualsT0(label);
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
    // zeroT0();
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
        setT0T2();
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
        lt();
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
        gt();
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
        setT2T0();
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
        // if(list[*tracker].type == IDENTIFIER)
        // {
        //     addVar(list[*tracker].token);
        // } else {
        //     addiCumulative(atof(list[*tracker].token));
        // }
        zeroT0();
        double a = T(list, tracker);
        addt0t1();
        double b = Eline(list, tracker);
        return a + b;
    }
    return -1;
}

double  T(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        if(list[*tracker].type == IDENTIFIER)
        {
            addVarT1(list[*tracker].token);
        } else {
            addiT1(atof(list[*tracker].token));
        }
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
    int isId = 0, idPlace;
    if (!strcmp(list[*tracker].token, "*"))
    {
        (*tracker)++;
        if(list[*tracker].type == IDENTIFIER)
        {
            isId = 1;
            idPlace = *tracker;
        }
        double a = F(list, tracker);
        double b = Tline(list, tracker, a);
        if(isId)
            mulVar(list[idPlace].token);
        else
            mulCumulative(a);
        return b * a;
    }
    if (!strcmp(list[*tracker].token, "/"))
    {
        (*tracker)++;
        if(list[*tracker].type == IDENTIFIER)
        {
            isId = 1;
            idPlace = *tracker;
        }
        double a = F(list, tracker);
        double b = Tline(list, tracker, a);
        if(isId)
            divVar(list[idPlace].token);
        else
            divCumulative(a);
        return b / a;
    }
    if (list[*tracker].token[0] == '%')
    {
        (*tracker)++;
        double a = F(list, tracker);
        double b = Tline(list, tracker, a);
        return (double)((int)prevValue % (int)(a * b)) / prevValue;
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
    int isId = 0, idPlace;
    if (!strcmp(list[*tracker].token, "+"))
    {
        (*tracker)++;
        if(list[*tracker].type == IDENTIFIER)
        {
            isId = 1;
            idPlace = *tracker;
        }
        double a = T(list, tracker);
        addt0t1();
        double b = Eline(list, tracker);
        if(isId)
            addVar(list[idPlace].token);
        else
            addiCumulative(atof(list[idPlace].token));
        return b + a;
    }
    if (!strcmp(list[*tracker].token, "-"))
    {
        (*tracker)++;
        if(list[*tracker].type == IDENTIFIER)
        {
            isId = 1;
            idPlace = *tracker;
        }

        double a = T(list, tracker);
        subt0t1();
        double b = Eline(list, tracker);
        if(isId)
            subVar(list[idPlace].token);
        else
            subiCumulative(atof(list[idPlace].token));
        return b - a;
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
    if (list[*tracker].type == INT ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == LEXIC_ERR)
    {
        error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        T(list, tracker);
        Eline(list, tracker);
        return -1;
    }
    return 0;
}

int declaration(Tokens *list, int *tracker, int type)
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
    variables[numberOfVariables].type = type;
    variables[numberOfVariables].line = list[idPlace].line;
    variables[numberOfVariables].column = list[idPlace].column;
    variables[numberOfVariables].value = 0;
    numberOfVariables++;
    (*tracker)++;

    A(list, tracker, idPlace, type);

    if (list[*tracker].type != SEPARATOR)
    {
        error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    (*tracker)++;
    return 0;
}

int A(Tokens *list, int *tracker, int idPlace, int type)
{
    if (!strcmp(list[*tracker].token, "="))
    {
        (*tracker)++;
        double value;
        if (list[*tracker].type == CHAR)
        {
            value = list[*tracker].token[1];
            (*tracker)++;
        }
        else
            value = expression(list, tracker);

        variables[numberOfVariables - 1].isInitialized = 1;
        variables[numberOfVariables - 1].value = value;
        sw(variables[numberOfVariables - 1].name);
        Aline(list, tracker, type);
        return 0;
    }
    if (!strcmp(list[*tracker].token, ","))
    {
        Aline(list, tracker, type);
    }
    return 0;
}

int Aline(Tokens *list, int *tracker, int type)
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
        variables[numberOfVariables].type = type;
        variables[numberOfVariables].line = list[idPlace].line;
        variables[numberOfVariables].column = list[idPlace].column;
        numberOfVariables++;
        (*tracker)++;
    }

    A(list, tracker, idPlace, type);

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

    char label[20];
    char initLabel[20];
    if (!loop)
    {
        sprintf(label, "endif%d:", ifNum++);
    }
    else
    {
        sprintf(initLabel, "while%d", whileNum);
        sprintf(label, "endwhile%d:", whileNum++);
        writeText(initLabel);
        writeText(":\n");
    }
    expression(list, tracker);

    jumpEqualsT0(label);
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
    if(loop)
    {
        zeroT0();
        jumpEqualsT0(initLabel);
    }
    writeText(label);
    writeText("\n");
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
        setValue(list[*tracker].token, getValue(list[*tracker].token) + 1);
        sw(list[*tracker].token);
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
        setValue(list[*tracker].token, getValue(list[*tracker].token) - 1);
        sw(list[*tracker].token);
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

        setValue(list[idPlace].token, getValue(list[idPlace].token) + 1);
        sw(list[idPlace].token);

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

        setValue(list[idPlace].token, getValue(list[idPlace].token) - 1);
        sw(list[idPlace].token);

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
        value = (int)list[*tracker].token[1];
        (*tracker)++;
    }
    else
    {
        value = expression(list, tracker);
    }
    sw(list[idPlace].token);

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
    setValue(list[idPlace].token, value);
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
        int type = 0;
        if (!strcmp(list[*tracker].token, "float"))
            type = 1;
        if (!strcmp(list[*tracker].token, "char"))
            type = 2;
        (*tracker)++;
        declaration(list, tracker, type);
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

int warning(int code, int line, int column)
{
    if (line == -1)
        printf("$: ");
    else
        printf("%d:%d: ", line, column);
    printf("Warning: ");
    switch (code)
    {
    case NOT_USD_ERR:
        printf("Variable not used\n");
        break;
    }
}

int error(int code, int line, int column)
{
    hasError = 1;
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
            warning(NOT_USD_ERR, variables[i].line, variables[i].column);
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

void setValue(char *variable, double value)
{
    for (int i = 0; i < numberOfVariables; i++)
    {
        if (!strcmp(variables[i].name, variable))
        {
            variables[i].value = value;
            return;
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

void gLoadImmediateVar(char *var)
{
    writeText("li ");
    writeText(var);
    writeText(",$t0\n");
}

void sw(char *label)
{
    writeText("sw $t0,");
    writeText(label);
    writeText("\n");
}

void writeText(char *str)
{
    if (textPtr + strlen(str) >= textSize)
    {
        textSize += 100;
        text = (char *)realloc(text, textSize * sizeof(char));
    }

    for (int i = 0; i < strlen(str); i++)
    {
        text[textPtr++] = str[i];
    }
}

void writeData(char *str)
{
    if (dataPtr + strlen(str) >= dataSize)
    {
        dataSize += 100;
        data = (char *)realloc(data, dataSize * sizeof(char));
    }

    for (int i = 0; i < strlen(str); i++)
    {
        data[dataPtr++] = str[i];
    }
}

void jumpEqualsT0(char *label)
{
    char newLabel[20];
    strcpy(newLabel, label);
    newLabel[strlen(newLabel) - 1] = '\0';
    writeText("beq $t0,$zero,");
    writeText(newLabel);
    writeText("\n");
}

void addiCumulative(double a)
{
    char buffer[46];
    sprintf(buffer, "addi $t0,$t0,%.0f\n", a);
    writeText(buffer);
}

void addVar(char *label)
{
    writeText("lw $t9,");
    writeText(label);
    writeText("\n");
    writeText("add $t0,$t0,$t9\n");
}

void addiT1(double a)
{
    char buffer[46];
    sprintf(buffer, "addi $t1,$zero,%.0f\n", a);
    writeText(buffer);
}

void addVarT1(char *label)
{
    writeText("lw $t9,");
    writeText(label);
    writeText("\n");
    writeText("add $t1,$zero,$t9\n");
}

void zeroT0()
{
    writeText("addi $t0,$zero,0\n");
    // writeText("addi $t1,$zero,1\n");
}

void zeroT1()
{
    writeText("addi $t1,$zero,1\n");
}

void subiCumulative(double a)
{
    char buffer[20];
    sprintf(buffer, "subi $t0,$t0,%.0f\n", a);
    writeText(buffer);
}

void subVar(char *label)
{
    writeText("lw $t9,");
    writeText(label);
    writeText("\n");
    writeText("sub $t0,$t0,$t9\n");
}

void mulCumulative(double a)
{
    char buffer[20];
    sprintf(buffer, "mul $t1,$t1,%.0f\n", a);
    writeText(buffer);
}

void mulVar(char *label)
{
    writeText("lw $t9,");
    writeText(label);
    writeText("\n");
    writeText("mul $t1,$t1,$t9\n");
}

void divCumulative(double a)
{
    char buffer[20];
    sprintf(buffer, "div $t1,$t1,%.0f\n", a);
    writeText(buffer);
}

void divVar(char *label)
{
    writeText("lw $t9,");
    writeText(label);
    writeText("\n");
    writeText("div $t1,$t1,$t9\n");
}

void addt0t1()
{
    writeText("add $t0,$t0,$t1\n");
}

void subt0t1()
{
    writeText("sub $t0,$t0,$t1\n");
}

void setT2T0()
{
    writeText("add $t2,$zero,$t0\n");
}

void setT0T2()
{
    writeText("add $t0,$zero,$t2\n");
}

void gt()
{
    writeText("gt $t2,$t2,$t0\n");
}

void lt()
{
    writeText("lt $t2,$t2,$t0\n");
}

