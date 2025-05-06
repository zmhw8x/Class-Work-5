#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
//This code has been changed once
#define SHM_KEY 6789
#define SHM_SIZE 64

int main() {
    int shmid;
    char *shm_ptr;
    pid_t pid;

    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);

    shm_ptr = (char *)shmat(shmid, NULL, 0);

    strcpy(shm_ptr, "Test 123");
    printf("Parent (%d) wrote\n", getpid());

    if ((pid = fork()) == -1) {
        exit(1);
    }

    if (pid == 0) {
        char *child_ptr = (char *)shmat(shmid, NULL, 0);

        printf("Child (%d) read: '%s'\n", getpid(), child_ptr);

        shmdt(child_ptr);
        exit(0);
    } else {
        wait(NULL);

        shmdt(shm_ptr);

        shmctl(shmid, IPC_RMID, NULL);
        printf("Parent (%d) cleaned up\n", getpid());
    }

    return 0;
}
