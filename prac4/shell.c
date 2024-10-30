#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define NL 1
#define END 2
#define SPECIALSYMBOLS "&|;><()"


int getWordsFromLine(FILE* inp, char*** mas_of_words, int* number_of_words, int* code) {
    int special_symbol_is_used = 0;
    int symbols_in_quotes = 0;
    long long int len_of_word = 1;
    int n = *number_of_words;

    n ++;
    (*mas_of_words) = (char**)realloc((*mas_of_words), n*sizeof(char*));
    (*mas_of_words)[n-1] = NULL;

    char* word = NULL;
    word = (char*)malloc(len_of_word*sizeof(char));
    word[0] = '\0';

    if (word == NULL || (*mas_of_words) == NULL) {
        return -1;
    }
    int buf;
    while (1) {
        if (!special_symbol_is_used) 
            buf = getc(inp);
        special_symbol_is_used = 0;
        if (buf == EOF || buf == '\n') {
            if (buf == EOF || (buf == '\n' && !symbols_in_quotes)) {
                *code = buf == EOF ? END : NL;
                if (symbols_in_quotes) {
                    fprintf(stderr, "\nThere are not closing quotes, I have closed them for you\n");
                }
                if (len_of_word > 1) {
                    (*mas_of_words)[n-1] = word;
                    return n;
                }
                else {
                    free(word);
                    return n;
                }
            }
            
        }
        else if (buf == '"') {
            if (symbols_in_quotes)
                symbols_in_quotes = 0;
            else 
                symbols_in_quotes = 1;
        }
        else if (isspace(buf) && !symbols_in_quotes) {
            if (len_of_word > 1) {
                (*mas_of_words)[n-1] = word;
                n++;
                (*mas_of_words) = (char**)realloc((*mas_of_words), n*sizeof(char*)); 
                (*mas_of_words)[n-1] = NULL;
                len_of_word = 1;
                word = (char*)malloc(len_of_word*sizeof(char));
                word[0] = '\0';
            }
        }
        else if (strchr(SPECIALSYMBOLS, buf) && !symbols_in_quotes) {
            if (len_of_word > 1) {
                (*mas_of_words)[n-1] = word;
                n++;
                (*mas_of_words) = (char**)realloc((*mas_of_words), n*sizeof(char*)); 
                (*mas_of_words)[n-1] = NULL;
                len_of_word = 2;
                word = (char*)malloc(len_of_word*sizeof(char));
            }
            else {
                len_of_word ++;
                word = (char*)realloc(word, len_of_word*sizeof(char));
            }
            word[0] = buf;
            word[1] = '\0';
            buf = fgetc(inp);
            if (buf != '&' && buf != '|' && buf != '>') {
                special_symbol_is_used = 1;
            }
            else {
                len_of_word ++;
                word = (char*)realloc(word, len_of_word*sizeof(char));
                word[1] = buf;
                word[2] = '\0';
            }
            (*mas_of_words)[n-1] = word;
            n++;
            (*mas_of_words) = (char**)realloc((*mas_of_words), n*sizeof(char*)); 
            (*mas_of_words)[n-1] = NULL;
            len_of_word = 1;
            word = (char*)malloc(len_of_word*sizeof(char));
            word[0] = '\0';
        }
        else {
            word[len_of_word-1] = buf;
            len_of_word++;
            word = (char*)realloc(word, len_of_word*sizeof(char));
            word[len_of_word-1] = '\0';
        }

    }
    return -1;
}

void freeMemory(char** mas_words, int len) {
    for (int i = 0; i < len; i++) {
        free(mas_words[i]);
    }
    free(mas_words);
}

void getAllWords(FILE* inp, char*** mas_of_words, int* number_of_words) {
    int code = 0;
    while (code != END) {
        (*number_of_words) = getWordsFromLine(inp, mas_of_words, number_of_words, &code);
        if (inp == stdin)
            fprintf(stdout, "\n");
        for (int i = 0; i < (*number_of_words); i++) {
            if ((*mas_of_words)[i] != NULL)
                fprintf(stdout, "%s\n", (*mas_of_words)[i]); 
        }
        freeMemory(*mas_of_words, *number_of_words);
        *mas_of_words = NULL;
        (*number_of_words) = 0;
        fprintf(stdout, "\n");
    }
}


int main(int argc, char** argv) {
    FILE* input = NULL;
    if (argc > 1) {
        input = fopen(argv[1], "r");
    }
    input = input ? input : stdin;
    
    int len = 0;
    char** mas_of_words = NULL;
    getAllWords(input, &mas_of_words, &len);
    
    return 0;
}