#ifndef _READING_H
#define _READING_H
    //headers
    #include <stdio.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <errno.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    
    void freeMemory(char** mas_words, int len);
    int getWordsFromLine(FILE* inp, char*** mas_of_words, int* number_of_words, int* code);
    void getAllCommands(FILE* inp, char*** mas_of_words, int* number_of_words);
    int executeCommands(char*** commands, int* number_of_commands);
    void changeDirection(char*** commands, int number_of_commands, int offset);
    int findConveyor(char*** commands, int number_of_commands);
    void executeConveyor(char*** commands, int number_of_commands, int number_of_processes_in_conveyor);
    int findBackgroundProcess(char*** commands, int number_of_commands);

    #define WD_BUFFER 1000
    #define NL 1
    #define END 2
    #define SPECIALSYMBOLS "&|;><()"
#endif
