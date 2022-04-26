#include <stdlib.h>
#include <stdio.h>
#include "../Token.h"
#include "../sintatico/sintatico.h"

void sintatico(Tokens *list, int numberOfTokens)
{
    int infantry = 0;
    for (int i = 0; i < numberOfTokens; i++)
    {
        if (list[i].type == SEPARATOR)
        {
            int size = i - infantry;
            int *command = cpCommand(size, list + i);
            analise(command, size);
            infantry = i + 1;
        }
    }
}

int *cpCommand(int size, Tokens *list)
{
    int *command = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        command[i] = list[i].type;
    }
    return command;
}

void analise(int *command, int size)
{
    printf("Command: \n");
    for(int i = 0; i < size; i++)
    {
        printf("%d ", command[i]);
    }
    printf("\n");
}