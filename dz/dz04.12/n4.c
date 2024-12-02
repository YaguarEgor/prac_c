#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <signal.h>

#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define __USE_XOPEN
#include <math.h>
//not full

#define NUMBER_SIZE 20

int main(int argc, char** argv) {
    if (argc < 6) {
        return -1;
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1) {
        return -1;
    }

    

    char* ptr = NULL;
    __uint128_t p = strtoull(argv[2], &ptr, 10);
    long double x0 = strtold(argv[3], &ptr);
    long double s = strtold(argv[4], &ptr);
    __uint128_t k = strtoull(argv[5], &ptr, 10);

    int sem = semget(IPC_PRIVATE, p, IPC_CREAT | 0777);
    semctl(sem, p, SETVAL, 0);  //необязательно, делается по умолчанию

    for (__uint128_t i = 0; i < p; i ++) {
        int son_mum = i;
        if (!fork()) {
            struct sembuf down = {son_mum, -1, 0};
            struct sembuf up  = {(son_mum+1)%p, 1, 0};

            for (__uint128_t j =0; j < k+1; j++) {
                semop(sem, &down, 1);
                lseek(fd, (son_mum*(k+1) + j)*NUMBER_SIZE + son_mum*sizeof(char), SEEK_SET);
                double cur = cos((x0+j*s)*(son_mum+1)/p*M_PI);
                char* str = malloc(NUMBER_SIZE);
                sprintf(str, "%.10g", cur);
                write(fd, str, strlen(str));
                if (j == k) {
                    char el = '\n';
                    write(fd, &el, sizeof(char));
                }
                semop(sem, &up, 1);
            }
            exit(0);
        }          
    }

    struct sembuf up  = {0, 1, 0};
    semop(sem, &up, 1);
    while (wait(NULL)!=-1);
    semctl(sem, 0, IPC_RMID, 0);
    return 0;
}