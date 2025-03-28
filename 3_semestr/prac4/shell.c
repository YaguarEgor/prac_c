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

void getRidOfBrackets(char*** commands, int* number_of_commands, int offset) {
    free((*commands)[offset]);
    int j = 1+offset;
    while ((*commands)[j]) {
        (*commands)[j-1] = (*commands)[j];
        j ++;
    }
    (*commands)[j-1] = NULL;
    if (strcmp((*commands)[j-2], ")")==0) {
        free((*commands)[j-2]);
        (*commands)[j-2] = NULL;
    }
    prepare_for_execution(*commands+offset, *number_of_commands-2);
}

void prepare_for_execution(char** commands_line, int number_of_words) {
    //ловим завершение фонового процесса
    pid_t pid_bcg;
    int status;
    if ((pid_bcg = waitpid(0, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("\nProcess %d exited with code %d\n", pid_bcg, WEXITSTATUS(status));
        }
        else {
            printf("\nProcess %d aborted by signal %d\n", pid_bcg, WTERMSIG(status));
        }
    }

    //проверка на пустую строку
    if (!(commands_line)[0]) {
		return;
	}

    //делим введенную строку на отдельные команды, если есть разделители ; && ||
    char*** commands = calloc(number_of_words,sizeof(char**));
    char** separators = calloc(number_of_words,sizeof(char*));
    int* number_of_commands_cur = calloc(number_of_words, sizeof(int));
    int number_of_commands = 0, number_of_separators = 0, num_of_opened_brackets = 0, status_of_finished_process;
    for (int i = 0; i<number_of_words && commands_line[i]; i++) {
        commands[i] = calloc(number_of_words, sizeof(char*));
        if (strcmp(commands_line[i], "(")==0) {
            num_of_opened_brackets++;
        }
        if (strcmp(commands_line[i], ")")==0) {
            num_of_opened_brackets--;
        }
        if ((strcmp(commands_line[i], "||")==0 || strcmp(commands_line[i], "&&")==0 || strcmp(commands_line[i], ";")==0) && i && !num_of_opened_brackets) {
            separators[number_of_separators++] = commands_line[i];
            number_of_commands++;
        }
        else {
            commands[number_of_commands][number_of_commands_cur[number_of_commands]++] = commands_line[i];
        }
    }
    number_of_commands++;
    //запускаем все найденные команды в нужном порядке, учитывая разделители
    for (int i = 0; i < number_of_commands; i++) {
        status_of_finished_process = executeCommands(&(commands[i]), &(number_of_commands_cur[i]));
        if (i == number_of_commands-1) {
            continue;
        }
        if (WIFEXITED(status_of_finished_process) && !WEXITSTATUS(status_of_finished_process) && !strcmp(separators[i], "||")) { //success
            while (i < number_of_separators && strcmp(separators[i], ";")) {
                i++;
                continue;
            }
        }
        else if ((!WIFEXITED(status_of_finished_process) || WEXITSTATUS(status_of_finished_process)) && !strcmp(separators[i], "&&")) {
            while (i < number_of_separators && strcmp(separators[i++], ";")) {
                i++;
                continue;
            }
        }
    }
    free(number_of_commands_cur);
    free(separators);
    for (int i = 0; i < number_of_words; i++) {
        free(commands[i]);
    }
    free(commands);
}

int executeCommands(char*** commands, int* number_of_commands) {
    if (!(*commands)[0]) {
		return 1;
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
                changeDirection(commands, *number_of_commands, 0);
                if (strcmp((*commands)[0], "(") == 0) {
                    getRidOfBrackets(commands, number_of_commands, 0);
                    while (wait(NULL)!=-1);
                    exit(0);
                }
                else {
                    execvp((*commands)[0], *commands);
                }
                

            }
            else if (number_of_processes_in_conveyor > 1) {
                executeConveyor(commands, *number_of_commands, number_of_processes_in_conveyor);
            }   
            perror("Error: Failed to find such command\n");
            exit(0); 
        }
        if (!is_bcg_process) {
            int status;
            waitpid(pid, &status, 0);
            return status;
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

void changeDirection(char*** commands, int number_of_commands, int offset) {
    int i = 0;
    char** cur_command = *commands+offset;
    while (i < number_of_commands-1) {
        if (!cur_command[i]) {
            break;
        }
        if (strcmp(cur_command[i], "<") == 0) {
            int fd_in = open(cur_command[i+1], O_RDONLY);
            if (fd_in != -1) {
                dup2(fd_in, 0);
                close(fd_in);
            }
            else {
                fprintf(stderr, "Error with changing input stream\n");
            }
            int j = i+2;
            while (cur_command[j]) {
                cur_command[j-2] = cur_command[j];
                j ++;
            }
            cur_command[j-2] = NULL;
            i--;
        }
        if (strcmp(cur_command[i], ">") == 0) {
            int fd_out = open(cur_command[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd_out != -1) {
                dup2(fd_out, 1);
                close(fd_out);
            }
            else {
                fprintf(stderr, "Error with changing error stream\n");
            }
            int j = i+2;
            while (cur_command[j]) {
                cur_command[j-2] = cur_command[j];
                j ++;
            }
            cur_command[j-2] = NULL;
            i--;
        }
        if (strcmp(cur_command[i], ">>") == 0) {
            int fd_out = open(cur_command[i+1], O_RDWR | O_APPEND | O_CREAT, 0777);
            if (fd_out != -1) {
                dup2(fd_out, 1);
                close(fd_out);
            }
            else {
                fprintf(stderr, "Error with changing error stream\n");
            }
            int j = i+2;
            while (cur_command[j]) {
                cur_command[j-2] = cur_command[j];
                j ++;
            }
            cur_command[j-2] = NULL;
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
                changeDirection(commands, number_of_commands-start, start);
                if (strcmp((*commands)[start], "(") == 0) {
                    int temp = number_of_commands-start;
                    getRidOfBrackets(commands, &temp, start);
                    while (wait(NULL)!=-1);
                    exit(0);
                }
                else {
                    execvp((*commands)[start], *commands+start);
                }
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
