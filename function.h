

#ifndef FUNCTION_H // Include guards to prevent multiple inclusion
#define FUNCTION_H
#include <stdbool.h>

extern char start_add[10];
extern char pgm_len[10];

struct symtab{
    char LABEL[20];
    char LOCCTR[20];
};
typedef struct symtab SYMTAB;

void printSYMTAB();
void deleteString(char *str);
char* searchOPCODE(char *opcode);
bool searchLABEL(char *label);
void insert(char *label, char *LOCCTR);
void segregate(const char *line, char *c1, char *c2, char *c3, char *c4, int i);
void setStart_add(char *value);
char *getStart_add();
bool searchSYMTAB(char *operand, char *locctr);
char* convert(char *operand);
void pass1(char *locc, char *label, char *opcode, char *operand);
void pass2(char *locc, char *label, char *opcode, char *operand);

#endif  // EXAMPLE_H
