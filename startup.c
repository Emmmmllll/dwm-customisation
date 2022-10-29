#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHM_SIZE  (size_t)1337
#define SHM_KEY   (key_t)187
#define SHM_PERM  (S_IRUSR | S_IWUSR)

void
die(const char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    exit(1);
}
void
clean(int id, int report){
    if(shmctl(id, IPC_RMID, NULL) < 0 && report)
        die("shmctl remove failed (%d)\n", id);
}

int main(int argc, char **argv){
    int shmid;
    int pid_comp;

    if(! (argc == 2 && strcmp(argv[1], "--no-comp") != 0)){
        if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_PERM | IPC_CREAT)) == -1){
            clean(shmid, 0);
            die("shmget failed (%d)\n", SHM_KEY);
        }
        if(argc == 2 && strcmp("-mk", argv[1]) == 0){
            printf("-shmid %d\n", shmid);
            exit(0);
        }
        if(argc == 2 && strcmp("-rm", argv[1]) == 0){
            clean(shmid, 1);
            puts("removed.");
            exit(0);
        }
        pid_comp = fork();
        if(pid_comp == 0){
            //start comp process
            // execv("/home/emil/dwm-customisation/bin/dwm-comp", NULL);
            exit(0);
        }        
    }
    else if(argc > 1)
        die("usage: dwm --no-comp\n");
    
    int pid_wm = fork();
    char shmidarg[11];
    sprintf(shmidarg, "%d", shmid);
    if(pid_wm == 0){
        char path[] = "/home/emil/dwm-customisation/bin/dwm-wm";
        char * debugflag = "-dbg";
        char * shmidflag = "-shmid";
        char * wm_args[5] = {">|<", debugflag, shmidflag, shmidarg, NULL};
        execv(path, wm_args);
        exit(0);
    }
    // cleanup
    waitpid(pid_wm, NULL, 0);
    kill(pid_comp, SIGKILL);

    clean(shmid, 1);
    return 0;
}