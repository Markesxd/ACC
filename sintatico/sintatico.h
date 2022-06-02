#define ID_EXP_ERR -1
#define SEP_EXP_ERR -2
#define PAR_EXP_ERR -3
#define OP_EXP_ERR -4
#define UNE_TKN_ERR -5 

struct b {
    char* name;
    int type;
    float value;
};

typedef struct b Variable;

void sintatico(Tokens *list, int n);

int analyse(Tokens *list, int *tracker);

int filterReservedWord(Tokens *list, int *tracker);

int condition(Tokens *list, int *tracker);

int declaration(Tokens *list, int *tracker);

int set(Tokens *list, int *tracker);

int error(int code, int line, int column);

int code(Tokens *list, int *tracker);

int command(Tokens *list, int *tracker);

int expression(Tokens *list, int *tracker);

int T(Tokens *list, int *tracker);

int E(Tokens *list, int *tracker);

int Eline(Tokens *list, int *tracker);

int Xline(Tokens *list, int *tracker);

int F(Tokens *list, int *tracker);

int Tline(Tokens *list, int *tracker);