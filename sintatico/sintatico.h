#define ID_EXP_ERR -1
#define SEP_EXP_ERR -2
#define PAR_EXP_ERR -3

struct b {
    char* name;
    int type;
    float value;
};

typedef struct b Variable;

void sintatico(Tokens *list, int n);

int analyse(Tokens *list, int *tracker);

int filterReservedWord(Tokens *list, int *tracker);

int handleIf(Tokens *list, int *tracker);

int handleIntDeclaration(Tokens *list, int *tracker);

int set(Tokens *list, int *tracker);

int error(int code);

float expression(Tokens *list, int *tracker);

float T(Tokens *list, int *tracker);

float Eline(Tokens *list, int *tracker);

float F(Tokens *list, int *tracker);

float Tline(Tokens *list, int *tracker);