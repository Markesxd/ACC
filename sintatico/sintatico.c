#include "../Token.h"
#include "../lexico/lexico.h"

void sintatico(Tokens *list, int numberOfTokens)
{
    int infantry = 0;
    for (int i = 0; i < numberOfTokens; i++)
    {
        if (list[i].type == SEPARATOR)
        {
            infantry = i + 1;
        }
    }
}