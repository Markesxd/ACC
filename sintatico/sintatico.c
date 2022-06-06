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

int expression(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        E(list, tracker);
        Xline(list, tracker);
        return 0;
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
        E(list, tracker);
        Xline(list, tracker);
        return -1;
    }
    error(EX_EXP_ERR, list[*tracker].line, list[*tracker].column);
    // (*tracker)++;
    return -1;
}

int Xline(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, ">") ||
        !strcmp(list[*tracker].token, "<") ||
        !strcmp(list[*tracker].token, "!") ||
        !strcmp(list[*tracker].token, "="))
    {
        if(!strcmp(list[*tracker].token, "!") ||
        !strcmp(list[*tracker].token, "="))
        {
            (*tracker)++;
            if(strcmp(list[*tracker].token, "="))
            {
                error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);

            }
            (*tracker)--;
        }
        (*tracker)++;
        X2line(list, tracker);
        E(list, tracker);
        Xline(list, tracker);
        return 0;
    }
    if (!strcmp(list[*tracker].token, "&"))
    {
        (*tracker)++;
        if (strcmp(list[*tracker].token, "&"))
        {
            error(OP_EXP_ERR, list[*tracker].line, list[*tracker].column);
            (*tracker)--;
        }
        (*tracker)++;

        E(list, tracker);
        Xline(list, tracker);
        return 0;
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
        E(list, tracker);
        Xline(list, tracker);
        return 0;
    }

    return 0;
}

int X2line(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "="))
    {
        (*tracker)++;
    }
    return 0;
}

int E(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        int ret = T(list, tracker);
        if(ret < 0)
            return ret;
        Eline(list, tracker);
        return 0;
    }
    return -1;
}

int T(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "(") ||
        !strcmp(list[*tracker].token, "-") ||
        !strcmp(list[*tracker].token, "!") ||
        list[*tracker].type == IDENTIFIER ||
        list[*tracker].type == FLOAT ||
        list[*tracker].type == INT ||
        list[*tracker].type == LEXIC_ERR)
    {
        int ret = F(list, tracker);
        if (ret < 0)
            return ret;
        ret = Tline(list, tracker);
        return ret;
    }
    error(TER_EXP_ERR, list[*tracker].line, list[*tracker].column);
    Tline(list, tracker);
    return -1;
}

int F(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "!") ||
        !strcmp(list[*tracker].token, "-"))
    {
        (*tracker)++;
    }
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
    if (list[*tracker].type == LEXIC_ERR)
    {
        (*tracker)++;
        return 0;
    }
    if (list[*tracker].type == EOF)
    {
        return -1;
    }
    error(TER_EXP_ERR, list[*tracker].line, list[*tracker].column);
    // (*tracker)++;
    return -1;
}

int Tline(Tokens *list, int *tracker)
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
        return 0;
    }
    if (!strcmp(list[*tracker].token, "*") ||
        !strcmp(list[*tracker].token, "/") ||
        list[*tracker].token[0] == '%')
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
        Tline(list, tracker);
        return 0;
    }
    // error(UNE_TKN_ERR, list[*tracker].line, list[*tracker].column);
    return UNE_TKN_ERR;    
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
    (*tracker)++;

    A(list, tracker);

    if (list[*tracker].type != SEPARATOR)
    {
        error(SEP_EXP_ERR, list[*tracker].line, list[*tracker].column);
        (*tracker)--;
    }
    // group[groupSize].name = list[(*tracker) - 1].token;
    // group[groupSize].type = list[*tracker - 1].type;
    // group[groupSize].value = 0;
    // groupSize++;
    (*tracker)++;
    return 0;
}

int A(Tokens *list, int *tracker)
{
    if (!strcmp(list[*tracker].token, "="))
    {
        (*tracker)++;

        if (list[*tracker].type == CHAR)
            (*tracker)++;
        else
            expression(list, tracker);

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
    if (!strcmp(list[*tracker].token, ","))
    {
        (*tracker)++;

        if (list[*tracker].type != IDENTIFIER)
        {
            error(ID_EXP_ERR, list[*tracker].line, list[*tracker].column);
            // (*tracker)--;
        }
        (*tracker)++;
    }

    A(list, tracker);

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
            return 0;
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
    // int idPlace = *tracker;
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

    if (list[*tracker].type == CHAR)
        (*tracker)++;
    else
        expression(list, tracker);

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
        printf("$: Sintatic error: ");
    else
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
    }
}