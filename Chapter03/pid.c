#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

char * const child_args[] = {
        "/bin/bash",
        NULL
};

int child_main(void* args){
    printf("In child process!\n");
    sethostname("alfonsNamespace", 15);           // add new hostname
    execv(child_args[0], child_args);
    return 1;
}

int main(){
    printf("Give root power first!\n");
    printf("Begin:\n");
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, SIGCHLD, NULL);
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | SIGCHLD, NULL);
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | CLONE_NEWIPC | SIGCHLD, NULL);
    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("End\n");
    return 0;
}
