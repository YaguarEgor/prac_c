//headers
#include "reading.h"
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

int main(int argc, char** argv) {
    signal(SIGINT, SIG_IGN);
    FILE* input = NULL;
    if (argc > 1) {
        input = fopen(argv[1], "r");
    }
    input = input ? input : stdin;
    
    int len = 0;
    char** mas_of_words = NULL;
    getAllCommands(input, &mas_of_words, &len);
    return 0;
}

int executeCommands(char*** commands, int* number_of_commands) {
    pid_t pid_bcg;
    int status;
    if ((pid_bcg = waitpid(0, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("\nProcess %d exited with code %d\n", pid_bcg, WEXITSTATUS(status));
        }
        else {
            printf("\nProcess %d aborted by signal %d\n", pid_bcg, WEXITSTATUS(status));
        }
    }
    
    (*commands) = (char**)realloc((*commands), (*number_of_commands+1)*sizeof(char*));
    (*commands)[*number_of_commands] = (char*)0;
    if (strcmp((*commands)[0], "cd") == 0) {
        if (*number_of_commands > 1) {
            if (chdir((*commands)[1]) != 0)
                perror("Error: Failed to change directory(1)\n");
        }
        else {
            if (chdir(getenv("HOME")) != 0) {
                perror("Error: Failed to change directory to parent(1)\n");
            }
        }
    }

    else {
        int is_bcg_process = findBackgroundProcess(commands, *number_of_commands);
        if (is_bcg_process > 1) {
            fprintf(stderr, "There can not be two '&'\n");
            return -1;
        } 
        pid_t pid;
        if ((pid = fork()) < 0) {
            fprintf(stderr, "Error");
            return -1;
        }
        else if (!pid) {
            int number_of_processes_in_conveyor = findConveyor(commands, *number_of_commands);
            signal(SIGINT, SIG_DFL);
            if (is_bcg_process) {
                signal(SIGINT, SIG_IGN);
                int fd = open("/dev/null", O_RDWR);
                dup2(fd, 0);
                close(fd);
            }
            if (number_of_processes_in_conveyor == 1) {
                changeDirection(commands, *number_of_commands);
                execvp((*commands)[0], *commands);
            }
            else if (number_of_processes_in_conveyor > 1) {
                executeConveyor(commands, *number_of_commands, number_of_processes_in_conveyor);
            }   
            perror("Error: Failed to find such command(2)\n");
            _exit(0); 
        }
        if (!is_bcg_process) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                return 1;
            }
        }
    }
    return 0;
}

void freeMemory(char** mas_words, int len) {
    for (int i = 0; i < len; i++) {
        free(mas_words[i]);
    }
    free(mas_words);
}

void changeDirection(char*** commands, int number_of_commands) {
    int i = 0;
    while (i < number_of_commands-1) {
        if (!(*commands)[i]) {
            break;
        }
        if (strcmp((*commands)[i], "<") == 0) {
            int fd_in = open((*commands)[i+1], O_RDONLY);
            if (fd_in != -1) {
                dup2(fd_in, 0);
                close(fd_in);
            }
            else {
                fprintf(stderr, "Error with changing input stream\n");
            }
            int j = i+2;
            while ((*commands)[j]) {
                (*commands)[j-2] = (*commands)[j];
                j ++;
            }
            (*commands)[j-2] = NULL;
            i--;
        }
        if (strcmp((*commands)[i], ">") == 0) {
            int fd_out = open((*commands)[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd_out != -1) {
                dup2(fd_out, 1);
                close(fd_out);
            }
            else {
                fprintf(stderr, "Error with changing error stream\n");
            }
            int j = i+2;
            while ((*commands)[j]) {
                (*commands)[j-2] = (*commands)[j];
                j ++;
            }
            (*commands)[j-2] = NULL;
            i--;
        }
        if (strcmp((*commands)[i], ">>") == 0) {
            int fd_out = open((*commands)[i+1], O_RDWR | O_APPEND | O_CREAT, 0777);
            if (fd_out != -1) {
                dup2(fd_out, 1);
                close(fd_out);
            }
            else {
                fprintf(stderr, "Error with changing error stream\n");
            }
            int j = i+2;
            while ((*commands)[j]) {
                (*commands)[j-2] = (*commands)[j];
                j ++;
            }
            (*commands)[j-2] = NULL;
            i--;
        }
        i++;
    }
    return;
}

int findConveyor(char*** commands, int number_of_commands) {
    int number_of_processes_in_conveyor = 1;
    if (strcmp((*commands)[0], "|")==0) {
        return -1;
    }
    for (int i = 1; (*commands)[i]; i++) {
        if (strcmp((*commands)[i], "|")==0) {
            (*commands)[i] = NULL;
            number_of_processes_in_conveyor++;
        }
    }
    return number_of_processes_in_conveyor;
}

void executeConveyor(char*** commands, int number_of_commands, int number_of_processes_in_conveyor) {
    int fd[2];
    int start = 0;
    for (int i = 0; i < number_of_processes_in_conveyor; i++) {
        pipe(fd);
        switch (fork()) {
            case -1:
                perror("Error: Ups....\n");
                _exit(0);
            case 0:
                if (i != number_of_processes_in_conveyor-1) {
                    dup2(fd[1], 1);
                }
                close(fd[0]);
                close(fd[1]);
                changeDirection(commands, number_of_commands);
                execvp((*commands)[start], *commands+start);
                perror("Error: Failed to make conveyor\n");
                _exit(0);
        }
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        while ((*commands)[start++] != NULL);
    }
    while (wait(NULL) != -1);
    _exit(0);
}

int findBackgroundProcess(char*** commands, int number_of_commands) {
    int number_of_amps = 0;
    int i =0;
    while ((*commands)[i]) {
        if (strcmp((*commands)[i], "&") == 0) {
            free((*commands)[i]);
            (*commands)[i] = NULL;
            number_of_amps++;
            int j = i+1;
            while ((*commands)[j]) {
                (*commands)[j-1] = (*commands)[j];
                j ++;
            }
            (*commands)[j-1] = NULL;
            i--;
        } 
        i++;
    }
    return number_of_amps;
}

