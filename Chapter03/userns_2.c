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

char *const child_args[] = {
        "/bin/bash",
        NULL
};

void set_map(const char *format, pid_t pid, int inside_id, int outside_id, int length) {
    char path[256];
    sprintf(path, format, getpid());
    FILE *fp = fopen(path, "w");
    fprintf(fp, "%d %d %d", inside_id, outside_id, length);
    fclose(fp);
}

void set_uid_map(pid_t pid, int inside_id, int outside_id, int length) {
    char path[256];
    sprintf(path, "/proc/%d/uid_map", getpid());
    FILE *fp = fopen(path, "w");
    fprintf(fp, "%d %d %d", inside_id, outside_id, length);
    fclose(fp);
}

void set_gid_map(pid_t pid, int inside_id, int outside_id, int length) {
    char path[256];
    sprintf(path, "/proc/%d/gid_map", getpid());
    FILE *fp = fopen(path, "w");
    fprintf(fp, "%d %d %d", inside_id, outside_id, length);
    fclose(fp);
}

int child_main(void *args) {
    printf("In child process!\n");
//    sethostname("alfonsNamespace", 15);           // add new hostname
//    mount("proc", "/proc", "proc", 0, NULL);        // mount function

//    set_map("/proc/%d/uid_map", getpid(), 0, 1000, 1);
//    set_map("/proc/%d/gid_map", getpid(), 0, 1000, 1);

    set_uid_map(getpid(), 0, 1000, 1);
    set_gid_map(getpid(), 0, 1000, 1);

    printf("eUID = %d, eGID = %d; ", getuid(), getgid());       // ubuntu 4.15.0-47 eGID = 65534
    cap_t caps = cap_get_proc();
    printf("capability: %s\n", cap_to_text(caps, NULL));
    execv(child_args[0], child_args);
    return 1;
}

int main() {
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
