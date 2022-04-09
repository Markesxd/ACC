#define LEXIC_ERR -1
#define RESERVED_WORD 0
#define IDENTIFIER 1
#define INT 2
#define SEPARATOR 3
#define OPERATOR 4
#define FLOAT 5
#define OPEN_PARENTESIS 6
#define CLOSE_PARENTESIS 7
#define OPEN_CURLY 8
#define CLOSE_CURLY 9

struct a {
    char token[30];
    int line;
    int column;
    int type;
};

typedef struct a Tokens; 
