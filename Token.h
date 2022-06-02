#define LEXIC_ERR -2
#define RESERVED_WORD 0
#define IDENTIFIER 1
#define INT 2
#define SEPARATOR 3
#define OPERATOR 4
#define FLOAT 5
#define DELIMITER 6
#define STRING 7

struct a {
    char token[30];
    int line;
    int column;
    int type;
};

typedef struct a Tokens; 
