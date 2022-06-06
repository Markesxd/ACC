#define ID_EXP_ERR -1
#define SEP_EXP_ERR -2
#define PAR_EXP_ERR -3
#define OP_EXP_ERR -4
#define UNE_TKN_ERR -5 
#define CUR_EXP_ERR -6
#define EX_EXP_ERR -7
#define TER_EXP_ERR -8
#define LP_ONL_ERR - 9

struct b {
    char* name;
    int type;
    float value;
};

typedef struct b Variable;

void sintatico(Tokens *list, int n);

int analyse(Tokens *list, int *tracker);

int filterReservedWord(Tokens *list, int *tracker);

int condition(Tokens *list, int *tracker, int loop, int inLoop);

int loop(Tokens *list, int *tracker);

int block(Tokens *list, int *tracker, int inLoop);

int bline(Tokens *list, int *tracker, int inLoop);

int declaration(Tokens *list, int *tracker);

int A(Tokens *list, int *tracker);

int Aline(Tokens *list, int *tracker);

int charDeclaration(Tokens *list, int *tracker);

int B(Tokens *list, int *tracker);

int Bline(Tokens *list, int *tracker);

int set(Tokens *list, int *tracker, int inFor);

int error(int code, int line, int column);

int code(Tokens *list, int *tracker);

int command(Tokens *list, int *tracker, int inLoop);

int expression(Tokens *list, int *tracker);

int T(Tokens *list, int *tracker);

int E(Tokens *list, int *tracker);

int Eline(Tokens *list, int *tracker);

int Xline(Tokens *list, int *tracker);

int X2line(Tokens *list, int *tracker);

int F(Tokens *list, int *tracker);

int Tline(Tokens *list, int *tracker);