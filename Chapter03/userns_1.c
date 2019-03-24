#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/capability.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

char * const child_args[] = {
        "/bin/bash",
        NULL
};

int child_main(void* args){
    printf("In child process!\n");
//    sethostname("alfonsNamespace", 15);           // add new hostname
//    mount("proc", "/proc", "proc", 0, NULL);        // mount function
    cap_t caps;
    printf("eUID = %d, eGID = %d; ",getuid(), getgid());
    caps = cap_get_proc();
    printf("capability: %s\n", cap_to_text(caps, NULL));
    execv(child_args[0], child_args);
    return 1;
}

int main(){
    printf("Give root power first!\n");
    printf("Begin:\n");
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, SIGCHLD, NULL);
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | SIGCHLD, NULL);
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | CLONE_NEWIPC | SIGCHLD, NULL);
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | SIGCHLD, NULL);
//    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);
    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUSER | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("End\n");
    return 0;
}
