/* Advent of Code 2024 - Day 3 Second Half */

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
        
        const char *str_reg_mul = "^mul\\([[:digit:]]{1,3}\\,[[:digit:]]{1,3}\\)";
        const char *str_reg_do = "^do\\(\\)";
        const char *str_reg_dont = "^don't\\(\\)";
        
        regex_t preg_mul;
        regex_t preg_do;
        regex_t preg_dont;
        
        int err_0 = regcomp(&preg_mul, str_reg_mul, REG_NOSUB | REG_EXTENDED);
        int err_1 = regcomp(&preg_do, str_reg_do, REG_NOSUB | REG_EXTENDED);
        int err_2 = regcomp(&preg_dont, str_reg_dont, REG_NOSUB | REG_EXTENDED);

        if (err_0 != 0 || err_1 != 0 || err_2 != 0) {
                printf("error when interpreting regex\n");
                exit(EXIT_FAILURE);
        }

        FILE *input;
        input = fopen("input", "r");
        
        if (input == NULL) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
        }
        
        int mul_cmp;
        int mul_do;
        int mul_dont;

        int mul_ret = 0;
        int is_mul = 1;

        while (!feof(input)) {
                emplchar(fgetc(input), str, MAX_CHAR - 1);

                mul_cmp = regexec(&preg_mul, str, 0, NULL, 0);  
                mul_do = regexec(&preg_do, str, 0, NULL, 0);  
                mul_dont = regexec(&preg_dont, str, 0, NULL, 0);  
        
                if (mul_do == 0)
                        is_mul = 1;

                if (mul_dont == 0)
                        is_mul = 0;

                if (mul_cmp == 0 && is_mul == 1) {
                        int a = atoi(&str[4]);
                        int b = atoi(strchr(str, ',') + 1); 
                        mul_ret += a * b;
                }
        }

        printf("mul: %i\n", mul_ret);

        regfree(&preg_mul);
        regfree(&preg_do);
        regfree(&preg_dont);

        exit(EXIT_SUCCESS);
}
