#include "function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    char label[20];
    char opcode[20];
    char operand[20];
    char line[100];
    char locc[10];
    
    pass1(locc, label, opcode, operand);
    pass2(locc, label, opcode, operand);
    
    
    
    return 1;
}
