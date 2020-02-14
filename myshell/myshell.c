#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

void start(char **words){
    pid_t  pid;
    int    status;
    if ((pid = fork()) < 0) {     /* fork a child process           */
         printf("*** ERROR: forking child process failed\n");
         exit(1);
    }
    else if (pid == 0) {          /* for the child process:         */
         if (execvp(words[1], &words[1]) < 0) {     /* execute the command  */
              printf("*** ERROR: exec failed\n");
              exit(1);
         }
    }
    else {                                  /* for the parent:      */
        printf("myshell: process %d started\n", pid);
    }
}

void run(char *file, char *words[100]){
    pid_t  pid;
    int    status;
    if ((pid = fork()) < 0) {     /* fork a child process           */
         printf("*** ERROR: forking child process failed\n");
         exit(1);
    }
    else if (pid == 0) {          /* for the child process:         */
         if (execvp(words[1], &words[1]) < 0) {     /* execute the command  */
              printf("*** ERROR: exec failed\n");
              exit(1);
         }
    }
    else {                                  /* for the parent:      */
         while (waitpid(pid, &status, 0) != pid);       /* wait for completion  */
         printf("myshell: process %d exited normally with status %d\n", pid, status);
              
    }

}

void killer(int pid, int sig){
    int ret = kill(pid, sig);

}


int main(int argc, char *argv[]){

    char quit[BUFSIZ];
    char exit[BUFSIZ];
    strcpy(quit, "quit");
    strcpy(exit, "exit");
    
    while(1){
        printf("myshell> ");
        char buf[4096]; 
        fgets(buf, 4096, stdin);  

        //strtok to get input
        int nwords = 0;
        char *words[100];
        char *ptr = strtok(buf," \t\n");
	    while(ptr != NULL)
	    {
            words[nwords] = ptr;
	    	ptr = strtok(0," \t\n");
            nwords++;

	    }
        words[nwords] = 0;
        //handles blank and skips
        if (strcmp(buf, "\n") == 0){
            continue;
        }

        //quit and exit 
        if (strcmp(words[0], quit) ==0 || strcmp(words[0], exit) == 0){
            printf("Exiting shell\n");
            return 0;
        }

        //checking if matches command
        if (strcmp(words[0], "wait")==0){
            pid_t cpid;
            int status;
            cpid = wait(&status);
            
            if (status == 0){
                printf("myshell: process %d exited normally with status %d\n", cpid, status);
            }
            else if(cpid == -1){
                printf("myshell: process %d exited abnormally with signal %s: %s\n", cpid, strsignal(WTERMSIG(status)), strerror(errno)); //
            }
            else{
                printf("myshell: no processes left\n");
            }
        }
        else if (strcmp(words[0], "run")==0){
            run(words[0], &words[0]);
        }
        else if (strcmp(words[0], "start")==0){
            start(words);
        }
        else if (strcmp(words[0], "kill")==0){ //stop kill adn continue are same function but different signals
            killer(atoi(words[1]), SIGKILL);
            printf("myshell: process %d killed\n", atoi(words[1]));
        }
        else if (strcmp(words[0], "stop")==0){
            killer(atoi(words[2]), SIGSTOP);
            printf("myshell: process %d stopped\n", atoi(words[1]));
        }
        else if (strcmp(words[0], "continue")==0){
            killer(atoi(words[2]), SIGCONT);
            printf("myshell: process %d continued\n", atoi(words[1]));
        }
        else{
            printf("unknown command: %s\n", buf);
    
        }

    }
    
    

}
