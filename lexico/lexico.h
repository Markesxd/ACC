int eot(char c);

int eol(char c);

void lexico(char* code);

int typify(char *token, char **vocab);

int isReservedWord(char *token, char** vocab);

int isInt(char *token);

int isFloat(char *token);

int isSeparator(char *token);

int isOperator(char *token);

int isValidIdentifier(char *token);

int regex(char *expression, char *string);

