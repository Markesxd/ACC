int eot(char c);

int eol(char c);

void* lexico(char *code, int *n);

int typify(char *token, char **vocab);

int isReservedWord(char *token, char **vocab);

void printTokens(void *list, int n);

void printError(void *data, int n);

int isChar(char c);

int isInt(char *token);

int isFloat(char *token);

int isSeparator(char *token);

int isOperator(char *token);

int isValidIdentifier(char *token);

int isDelimiter(char c);

int isString(char c);

int regex(char *expression, char *string);
