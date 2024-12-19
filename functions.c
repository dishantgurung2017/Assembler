#include "function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char start_add[10] = {'\0'};
char pgm_len[10] = {'\0'};
int size = 0;
SYMTAB sym[100];

void printSYMTAB(){
    for(int i = 0; i < size; i++){
        printf("%s %s\n",sym[i].LABEL,sym[i].LOCCTR);
    }
}

void deleteString(char *str) {
    memset(str, 0, strlen(str));
}

char* searchOPCODE(char *opcode){
    FILE *read;
    read = fopen("optab.txt", "r");
    char line[50], c1[10], c2[10];
    char *c3 = malloc(strlen(c2) + 1);
    while(fgets(line, sizeof(line), read) != NULL){
        memset(c1, '\0', sizeof(c1));
        memset(c2, '\0', sizeof(c2));
        int n = sscanf(line, "%s %s", c1, c2);
        if (strcmp(opcode, c1) == 0){
            strcpy(c3, c2);
            return c3;
        }
    }
    return "NULL";
    
}

/*char* searchOPCODE(char *opcode)
{
    FILE *read;
    read = fopen("optab.txt", "r");
    char line[50];
    while(fgets(line, sizeof(line), read) != NULL)
    {
        char *token = strtok(line, " ");
        if (token != NULL)
        {
            if (strcmp(opcode, token) == 0)
                return strtok(NULL, " ");
        }
    }
    return "NULL";
}*/

bool searchLABEL(char *label){
    
    for(int i = 0; i < size; i++){
        if(strcmp(label, sym[i].LABEL) == 0){
            return true;
        }
    }
    return false;
}

void insert(char *label, char *LOCCTR){
    if(!searchLABEL(label)){
        //printf("%d",1);
        strcpy(sym[size].LABEL, label);
        strcpy(sym[size].LOCCTR, LOCCTR);
        size++;
    }
}


void segregate(const char *line, char *c1, char *c2, char *c3, char *c4, int i) {
    c2[0] = '\0';
    c3[0] = '\0';
    int num_matched = sscanf(line, "%s %s %s %s", c1, c2, c3, c4);
    if(i == 1){
        if(num_matched == 3){
            strcpy(c4, c3);
            strcpy(c3, c2);
            strcpy(c2, c1);
            strcpy(c1, "NULL");

        }
        if (num_matched == 2) {
            strcpy(c4, c2);
            strcpy(c3, c1);
            strcpy(c2, "NULL");
            strcpy(c1, "NULL");
        }
        if(num_matched == 1){
            strcpy(c4, "NULL");
            strcpy(c3, c1);
            strcpy(c2, "NULL");
            strcpy(c1, "NULL");
        }
    }
    if(i == 2){
        if(num_matched == 3){
            strcpy(c4, c3);
            strcpy(c3, c2);
            strcpy(c2, "NULL");

        }
        if (num_matched == 2) {
            strcpy(c4, "NULL");
            strcpy(c3, c2);
            strcpy(c2, "NULL");
        }
    }
    
}

void setStart_add(char *value){
    strcpy(start_add, value);
}

char *getStart_add(){
    return start_add;
}

bool searchSYMTAB(char *operand, char *locctr){
    for(int i = 0; i < size; i++){
        if(strcmp(sym[i].LABEL,operand) == 0){
            strcpy(locctr,sym[i].LOCCTR);
            return true;
        }
    }
    return false;
}

char* convert(char *operand){
    char c[10];
    char *s = malloc(10 * sizeof(char));
    //char s[20] = {'\0'};
    int i = 2, j = 0;
    if(operand[0] == 'X'){
        while(operand[i+1] != '\0'){
            s[j] = operand[i];
            i++;
            j++;
        }
        s[j] = '\0';
    }
    else if(operand[0] == 'C'){
        int i = 3;
        j = operand[2];
        sprintf(c, "%X", j);
        strcpy(s,c);
        while(operand[i+1] != '\0'){
            j = operand[i];
            sprintf(c, "%X", j);
            strcat(s,c);

            i++;
        }
    }
    return s;
}

void pass1(char *locc, char *label, char *opcode, char *operand){
    char line[100], LOCCTR[10], p[10];
    int l_no = 1;
    FILE *read;
    FILE *write;

    read = fopen("input.txt", "r");
    if(read == NULL){
        perror("Error opening file");
        return;
    }

    write = fopen("intermediate_file.txt", "w");
    if (write == NULL) {
        printf("Error opening the output file.\n");
        fclose(read);
        return;
    }

    while(fgets(line, sizeof(line), read) != NULL){
        //printf("%d", l_no);
        segregate(line, locc, label, opcode, operand, 1);
        //printf("%s\n", opcode);
        if(strcmp(opcode, "END") == 0){
            //printf("%d", 1);
            break;
        }
        if(l_no == 1){
            if(strcmp(opcode, "START") == 0){
                //printf("%d", 1);
                strcpy(start_add, operand);
                strcpy(LOCCTR, start_add);
                //printf("%s",start_add);
                //printf("%s",line);
                
                fprintf(write, "%s %s", LOCCTR, line);
                //printf("%s",operand);
                //fclose(write);
            }else strcpy(LOCCTR, "0");
        }
        else{
            fprintf(write, "%s %s", LOCCTR, line);
            //printf("%s\n",opcode);
            if(strcmp(label,"NULL") != 0){
                if(searchLABEL(label)){
                    printf("%s","Error!Duplicate symbol");
                }else insert(label, LOCCTR);
            }
            if(searchOPCODE(opcode) != "NULL"){
                //if(strcmp(opcode, "JEQ") == 0)
                  //  printf("%d",1);
                long decimal = strtol(LOCCTR, NULL, 16);
                decimal += 3;
                sprintf(LOCCTR, "%lX", decimal);
            }
            else if(strcmp(opcode, "WORD") == 0){
                //printf("%d", 0);
                long decimal = strtol(LOCCTR, NULL, 16);
                decimal += 3;
                sprintf(LOCCTR, "%lX", decimal);
            }
            else if(strcmp(opcode, "RESW") == 0){
                long decimal = strtol(LOCCTR, NULL, 16);
                decimal += (3 * atoi(operand));
                sprintf(LOCCTR, "%lX", decimal);
            }
            else if(strcmp(opcode, "RESB") == 0){
                long decimal = strtol(LOCCTR, NULL, 16);
                decimal += atoi(operand);
                sprintf(LOCCTR, "%lX", decimal);
            }
            else if(strcmp(opcode, "BYTE") == 0){
                int len = strlen(operand) - 3;
                if(operand[0] == 'C'){
                    long decimal = strtol(LOCCTR, NULL, 16);
                    decimal += len;
                    sprintf(LOCCTR, "%lX", decimal);
                }
                else if(operand[0] == 'X'){
                    int length = len/2;
                    long decimal = strtol(LOCCTR, NULL, 16);
                    decimal += length;
                    sprintf(LOCCTR, "%lX", decimal);
                }
            }
            else printf("%s","ERROR!Invalid opcode");
            //write = fopen("intermediate_file.txt", "w");
            
        }
        l_no++;
    }
    //write = fopen("intermediate_file.txt", "w");
    fprintf(write, "%s %s", LOCCTR, line);
    fclose(read);
    fclose(write);
    long loc = strtol(LOCCTR, NULL, 16);
    long st = strtol(start_add, NULL, 16);
    long p_len = loc - st;
    sprintf(pgm_len, "%lX", p_len);
    //printf("%s",p);
    printSYMTAB();
    printf("%d",size);
}

void pass2(char *locc, char *label, char *opcode, char *operand){
    char line[100], locctr[20];
    int count;
    char op[20] = {'\0'};
    char text[100], st[20], text_len[10];
    char ob_code[20];
    FILE *read;
    FILE *write;
    read = fopen("intermediate_file.txt", "r");
    if(read == NULL){
        perror("Error opening file");
        return;
    }

    write = fopen("object_pgm.txt", "w");
    if (write == NULL) {
        printf("Error opening the output file.\n");
        fclose(read);
        return;
    }

    if(fgets(line, sizeof(line), read)){
        segregate(line, locc, label, opcode, operand, 2);
        fprintf(write, "%s%s", "H^", label);
        for(int i = strlen(label); i < 6; i++)
            fprintf(write, "%c", '*');
        fprintf(write, "%s", "^");
        for(int i = 0; i < (6 - strlen(start_add)); i++)
            fprintf(write, "%c", '0');
        fprintf(write, "%s%s", start_add, "^");
        for(int i = 0; i < (6 - strlen(pgm_len)); i++)
            fprintf(write, "%c", '0');
        fprintf(write, "%s\n", pgm_len);
    }
    if(fgets(line, sizeof(line), read)){
        int check = 0;
        segregate(line, locc, label, opcode, operand, 2);
        while(check == 0){
            memset(text, '\0', sizeof(text));
            count = 0;

            if(strcmp(opcode, "END") == 0){
                break;
            }
            //segregate(line, locc, label, opcode, operand, 2);
        
            if((strcmp(opcode, "RESW") != 0) && (strcmp(opcode, "RESB") != 0)){
                fprintf(write, "%s", "T^");
                for(int i = 0; i < (6 - strlen(locc)); i++)
                    fprintf(write, "%c", '0');
                fprintf(write, "%s%s", locc, "^");
                strcpy(st,locc);
                while(count < 10 && ((strcmp(opcode, "RESW") != 0) && (strcmp(opcode, "RESB") != 0))){
                    strcpy(op,searchOPCODE(opcode));
                    if(strcmp(op, "NULL") != 0){
                        printf("%s", op);
                        if(strcmp(operand, "NULL") != 0){
                            if(searchSYMTAB(operand,locctr)){
                                char cat[10] = {'\0'};
                                strcpy(cat, op);
                                strcat(cat,locctr);
                                strcat(text,cat);
                                strcat(text,"^");
                            }
                            else{
                                char *token = strtok(operand,",");
                                if(searchSYMTAB(token,locctr)){
                                    char str[20];
                                    long num1 = strtol(locctr,NULL,16);
                                    long num2 = strtol("8000", NULL, 16);
                                    long sum = num1 + num2;
                                    sprintf(str, "%lX", sum);
                                    strcat(op, str);
                                    strcat(text, op);
                                    strcat(text, "^");
                                }
                            }
                            count++;
                        }else{
                            strcat(op, "0000");
                            strcat(text, op);
                            strcat(text, "^");
                            count++;
                        }
                    }else if(strcmp(opcode, "BYTE") == 0 ){
                        //printf("%d",1);
                        strcat(text, convert(operand));
                        strcat(text, "^");
                        count++;
                    }else if(strcmp(opcode, "WORD") == 0){
                        char hex[20];
                        sprintf(hex, "%X", atoi(operand));
                        for(int i = 0; i < (6 - strlen(hex)); i++)
                        strcat(text, "0");
                        strcat(text, hex);
                        strcat(text, "^");
                        count++;
                    }
                    if(fgets(line, sizeof(line), read))
                        segregate(line, locc, label, opcode, operand, 2);
                    if(strcmp(opcode, "END") == 0){
                        check = 1;
                        break;
                    }
                }
                long loc = strtol(locc, NULL, 16);
                long start = strtol(st, NULL, 16);
                long p_len = loc - start;
                sprintf(text_len, "%lX", p_len);
                fprintf(write, "%s%s", text_len, "^");
                fprintf(write, "%s\n", text);
            }else{
                if(fgets(line, sizeof(line), read))
                    segregate(line, locc, label, opcode, operand, 2);
                if(strcmp(opcode, "END") == 0){
                    check = 1;
                    break;
                }
            }
        }
            
    }
        
    fprintf(write, "%s", "E^");
    for(int i = 0; i < (6 - strlen(pgm_len)); i++)
            fprintf(write, "%c", '0');
    fprintf(write, "%s", start_add);

}
