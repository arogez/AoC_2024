/* Advent of Code 2024 - Day 1 First Half */

#include <stdio.h>
#include <stdlib.h>

#define N_ROW 1000
#define N_CHAR 5

int intcmp(const void *a, const void *b) 
{
        int cmp0 = *(int *)a;
        int cmp1 = *(int *)b;
        
        return cmp0 - cmp1;
}

void parse(FILE *input, int *l_0, int *l_1)
{
        char c = fgetc(input), buffer[6] = {0};
        int col = 0, row = 0, i = 0;
        int *ptr;

        while (!feof(input)) {
                if (c == '\n') row++;
                else if (c == ' ') { }
                else {
                        buffer[i] = c;
                        if (i == N_CHAR - 1) {
                                ptr = (col == 0) ? &l_0[row] : &l_1[row];
                                *ptr = atoi(buffer);
                                col = 1 - col;
                                i = 0;
                        } else i++;
                }

                c = fgetc(input);
        }
}

int main(int argc, char *argv[]) {

        int dist = 0;
        int list_0[N_ROW], list_1[N_ROW];
        
        FILE *input;
        input = fopen("input", "rb");

        if (input == NULL) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
        }

        parse(input, list_0, list_1);
        qsort(list_0, N_ROW, sizeof(int), intcmp);
        qsort(list_1, N_ROW, sizeof(int), intcmp);
         
        for (int i = 0; i < N_ROW; i++) {
                dist += abs(list_0[i] - list_1[i]);
        }
        
        printf("dist : %i\n", dist);
        
        fclose(input);
        return 0;
} 
