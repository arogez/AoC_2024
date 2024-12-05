/* Advent of Code 2024 - Day 1 First Half */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEVELS 8

void empl_line(FILE *input, char *line, int *len)
{
        char c = fgetc(input);

        if (c == EOF) 
                return;

        while (c != '\n') {
                line[*len] = c;
                (*len)++;
                c = fgetc(input);
        }
}

void parse_line(char* line, int *levels, const int str_len, int *arr_len)
{
        char level[3] = {0};
        int y = 0;
        
        for (int i = 0; i < str_len; i++) {
                if (line[i] == ' ') {
                        y = 0;
                        levels[*arr_len] = atoi(level);
                        (*arr_len)++;
                        memset(level, 0, sizeof(char) * 3);
                        continue;
                }

                if (i == str_len - 1) {
                        level[y] = line[i];
                        levels[*arr_len] = atoi(level);
                        (*arr_len)++;
                        continue;
                }

                level[y] = line[i];
                y++;
        }
}

int is_safe(int *levels, int len)
{
        int prev = levels[0];
        int diff = abs(levels[1] - prev);
        
        if (diff == 0 || diff > 3)
                return 0;

        if (levels[0] < levels[1]) {
                for (int i = 1; i < len - 1; i++) {
                        if (levels[i] > levels[i + 1]) 
                                return 0;
                
                        prev = levels[i];
                        diff = abs(levels[i+1] - prev);
                        
                        if (diff == 0 || diff > 3)
                                return 0;
                }
        }

        if (levels[0] > levels[1]) {
                for (int i = 1; i < len - 1; i++) {
                        if (levels[i] < levels[i + 1]) 
                                return 0;

                        prev = levels[i];
                        diff = abs(levels[i+1] - prev);

                        if (diff == 0 || diff > 3)
                                return 0;
                }
        }
 
        return 1; 
}

int main(int argc, char *argv[])
{
        FILE *input;
        input = fopen("input", "r");
        
        if (input == NULL) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
        }
        
        size_t nbytes = sizeof(char) * (MAX_LEVELS * 3);
        char *line = malloc(nbytes);
        
        int str_len = 0, arr_len = 0;
        int levels[MAX_LEVELS];

        int safe_count = 0;

        while (!feof(input)) {

                empl_line(input, line, &str_len);
                
                if (line[0] == '\0') {
                        break;
                }
                
                parse_line(line, levels, str_len, &arr_len);
                safe_count += is_safe(levels, arr_len);
               
                /* reset data before processing next line */
                memset(line, 0, nbytes);
                memset(levels, 0, sizeof(int) * MAX_LEVELS);
                str_len = 0, arr_len = 0;
        }
            
        printf("safe reports: %i\n", safe_count);

        free(line);
        return 0;
}
