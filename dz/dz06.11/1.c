#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    printf("Father process is started\n");
    pid_t ret_pid = fork();
    if (ret_pid == 0) {
        
        //execl("/bin/gcc", "gcc", "1.c", "-o", "1.exe", "-lm", (char*)0);

        char* args[] = {"gcc", "1.c", "-o", "1.exe", "-lm", (char*)0};
        execv("/bin/gcc", args);

        printf("Error\n");
    }
    wait(NULL);
    printf("everything is done\n");
    return 0;
}
