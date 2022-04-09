#include <stdio.h>
#include <regex.h>

int a(int b)
{
    return b;
}
int main(int argc,char *argv[]){
    regex_t reg;
    regcomp(&reg, argv[1], REG_EXTENDED);
    if(!regexec(&reg, argv[2], 0, NULL, 0))
        printf("bom\n");
    else  
        printf("ruim\n");
}