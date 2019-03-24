#include <unistd.h>
#include <stdio.h>

int main(){
    pid_t fpid;
    int count = 0;

    fpid = fork();
    if (fpid < 0){
        printf("error in fork!");
    } else if (fpid > 0){
        printf("I'm in parent. Process id is %d.\n", getpid());
    } else {
        printf("I'm in child. Process id is %d.\n", getpid());
    }
    return 0;
}