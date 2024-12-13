#include <stdlib.h>
#include <stdio.h>

#define bit(X)          (1 << X)

#define WORD_LEN        4
#define LINE_LEN        140
#define X_LIMIT         (LINE_LEN - WORD_LEN)

int pos_off[] = {1, LINE_LEN - 1, LINE_LEN, LINE_LEN + 1};
enum flags {X, M, A, S};

int xmas_count;

struct xmas {
        int type;
        int masks[4];
        int next_pos[4];
        struct xmas *next;
};

struct list {
        struct xmas *head;
};

struct list xmas_active = {0}; 
struct list xmas_store = {0};

const unsigned delete(struct xmas **head, void *node)
{
        struct xmas *ptr = (*head);
        struct xmas* prev = NULL; 	
        
        if ((*head) != NULL && (*head) == node) {
                (*head) = ptr->next;
                return 1;
        }

        while (ptr != NULL && ptr != node) {
                prev = ptr;
                ptr = prev->next; 
        }

        if (ptr == NULL) 
                return 0;

        prev->next = ptr->next;
        
        return 1;
}

void push(struct xmas **head, void *ptr) 
{
        if (ptr == NULL) 
                return;

        if (*head == NULL) {
                (*head) = ptr;
                (*head)->next = NULL;
                return;
        }

        struct xmas *new = ptr;

        new->next = (*head);
        (*head) = new;
}

void pop(struct xmas **head)
{
        if ((*head) == NULL)
                return;

        if ((*head)->next != NULL) 
                (*head) = (*head)->next;
        else 
                (*head) = NULL; 
}

const char ccmp(int mask, int type) 
{
        char c = '\0';
        
        if (type == X) {
                if (mask & bit(X)) c = 'M';
                if (mask & bit(M)) c = 'A';
                if (mask & bit(A)) c = 'S';
        }

        if (type == S) {
                if (mask & bit(S)) c = 'A';
                if (mask & bit(A)) c = 'M';
                if (mask & bit(M)) c = 'X';
        }

        return c;
}

const int node_clear(int *pos) 
{
        for (int i = 0; i < 4; i++) {
                if (pos[i] != 0) return 0;
        }

        return 1;
}

void resolve(struct xmas *head, const int pos, const char c) 
{
        struct xmas *ptr = head;

        while (ptr != NULL) {
                for (int i = 0; i < 4; i++) {
                        if (ptr->next_pos[i] == 0) {
                                continue;
                        }

                        if (ptr->next_pos[i] == pos) {
                                if (ccmp(ptr->masks[i], ptr->type) == c) {
                                        ptr->next_pos[i] += pos_off[i];
                                        if (ptr->type == X) ptr->masks[i] *= 2;
                                        if (ptr->type == S) ptr->masks[i] /= 2;
                                        
                                        if (ptr->masks[i] & bit(S) && ptr->type == X) {
                                                xmas_count++;
                                                ptr->next_pos[i] = 0;
                                                continue;
                                        }

                                        if (ptr->masks[i] & bit(X) && ptr->type == S) {
                                                xmas_count++;
                                                ptr->next_pos[i] = 0;
                                                continue;
                                        }

                                } else {
                                        ptr->next_pos[i] = 0;
                                }
                        }
                }
                
                if (node_clear(ptr->next_pos)) {
                        struct xmas *temp = ptr->next;
                        delete(&xmas_active.head, ptr);
                        push (&xmas_store.head, ptr);
                        ptr = temp;
                } else {
                        ptr = ptr->next;
                }
        }
}

void list_free(struct xmas *head) 
{
        struct xmas *ptr;

        while (head != NULL) {
                ptr = head; 
                head = ptr->next;
                free(ptr);
        }
}

void word_init(struct xmas *word, int pos, int row, char c)
{
        word->type = 0;
        word->next = NULL;
       
        if (c == 'X') word->type = X;
        if (c == 'S') word->type = S;

        for (int i = 0; i < 4; i++) {
                word->masks[i] = 0;
        }
        
        if ((pos - (row * LINE_LEN)) <= X_LIMIT) { 
                word->next_pos[0] = pos + pos_off[0];
                if (c == 'X') word->masks[0] |= bit(X);
                if (c == 'S') word->masks[0] |= bit(S);
        } else word->next_pos[0] = 0;

        if ((pos - (row * LINE_LEN)) >= 3 && row <= X_LIMIT) {
                word->next_pos[1] = pos + pos_off[1];
                if (c == 'X') word->masks[1] |= bit(X);
                if (c == 'S') word->masks[1] |= bit(S);
        } else word->next_pos[1] = 0;
        
        if (row <= X_LIMIT) {                     
                word->next_pos[2] = pos + pos_off[2];
                if (c == 'X') word->masks[2] |= bit(X);
                if (c == 'S') word->masks[2] |= bit(S);
        } else word->next_pos[2] = 0;

        if ((pos - (row * LINE_LEN)) <= X_LIMIT && row <= X_LIMIT) {
                word->next_pos[3] = pos + pos_off[3];
                if (c == 'X') word->masks[3] |= bit(X);
                if (c == 'S') word->masks[3] |= bit(S);
        } else word->next_pos[3] = 0;

}

int main(int argc, char *argv[]) {

        struct xmas *word;

        FILE *input;
        input = fopen("input", "r");

        if (input == NULL) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
        }

        char c = fgetc(input);
        int pos = 0;
        int row = 0;

        xmas_count = 0;

        while (!feof(input)) {
                if (c == 'X' || c == 'S') {
                        if (xmas_store.head) {
                                word = xmas_store.head;
                                pop(&xmas_store.head);
                        } else word = malloc(sizeof(struct xmas));
                        word_init(word, pos, row, c);
                        push(&xmas_active.head, word);
                }

                if (c == '\n') {
                        row++;
                        goto next_char;
                }
                
                resolve(xmas_active.head, pos, c);
                pos++;
                
                next_char: c = fgetc(input);
        }

        printf("count: %i\n", xmas_count);
       
        list_free(xmas_active.head);
        list_free(xmas_store.head);

        fclose(input);

        return EXIT_SUCCESS;
}
