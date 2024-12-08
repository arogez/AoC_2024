/* Advent of Code 2024 - Day 3 First Half */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define MAX_CHAR 13

void emplchar(char c, char *str, int len)
{
        for (int i = 0; i < len - 1; i++) {
                str[i] = str[i + 1]; 
        }

        str[len - 1] = c;
}

//regex definition: ^mul\([[:digit:]]{1,3}\,[[:digit:]]{1,3}\)
int main(int argc, char *argv[])
{
        char str[MAX_CHAR] = {0};
        const char *str_reg = "^mul\\([[:digit:]]{1,3}\\,[[:digit:]]{1,3}\\)";
       
        regex_t preg;
        
        int err = regcomp(&preg, str_reg, REG_NOSUB | REG_EXTENDED);

        if (err != 0) {
                printf("error when interpreting regex\n");
                exit(EXIT_FAILURE);
        }

        FILE *input;
        input = fopen("input", "r");
        
        if (input == NULL) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
        }
        
        int cmp;
        int mul = 0;
        
        while (!feof(input)) {
                emplchar(fgetc(input), str, MAX_CHAR - 1);

                cmp = regexec(&preg, str, 0, NULL, 0);  

                if (cmp == 0) {
                        int a = atoi(&str[4]);
                        int b = atoi(strchr(str, ',') + 1); 
                        mul += a * b;
                }
        }

        printf("mul: %i\n", mul);

        regfree(&preg);
        exit(EXIT_SUCCESS);
}
