#define ID_EXP_ERR -1
#define SEP_EXP_ERR -2

struct b {
    char* name;
    int type;
    int value;
};

typedef struct b Variable;

void sintatico(Tokens *list, int n);

int analise(Tokens *list, int *tracker);

int filterReservedWord(Tokens *list, int *tracker);

int handleIf(Tokens *list, int *tracker);

int handleIntDeclaration(Tokens *list, int *tracker);

int set(Tokens *list, int *tracker);

int error(int code);

int expression(Tokens *list, int *tracker);

int T(Tokens *list, int *tracker);

int Eline(Tokens *list, int *tracker);

int F(Tokens *list, int *tracker);

int Tline(Tokens *list, int *tracker);