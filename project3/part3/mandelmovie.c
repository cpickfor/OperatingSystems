#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>


// ./mandel -x -0.5024 -y -0.603 -s 0.0001 -m 1000 -H 1024 -W 1300

int counter = 0;
float scaleStart = 2.0000;
float scaleEnd = 0.0001;
int numFork = 0;


int main(int argc, char* argv[]){
    pid_t pid;
    int status;
    
    //check number of arguments is correct
    if(argc != 2) {
		printf("command shoulf follow: ./mandelmovie <#forks>\n");
		exit(1);
	}

    int forkNum = atoi(argv[1]); //get number of processes from command line
    float zoom = exp(log(scaleEnd/scaleStart)/51); //controll zoom
    printf("%f\n", zoom);

    //while loop for 50 images
    while (counter < 50){

        //check number of forks
        if(numFork < forkNum){
            //increment counters
            numFork++;
            counter++;

            //command
            char* command[100];
            command[0] = "./mandel";
            command[1] = "-x -0.5024";
	        command[2] = "-y -0.603";
	        command[3] = "-m 1000";
	        command[4] = "-H 1024";
	        command[5] = "-W 1300";
	        command[6] = malloc(100 * sizeof(char));	//zoom
	        command[7] = "-o";
	        command[8] = malloc(100 * sizeof(char));	//new picture
	        command[9] = NULL;
            //zoom asserted
            sprintf(command[6], "-s %f", scaleStart);
            //file name change
            sprintf(command[8], "mandel%d.bmp", counter);

            //convert float to string
            //char flags[BUFSIZ];
            //int ret = snprintf(flags, sizeof flags, " -x -0.5024 -y -0.603 -m 1000 -H 1024 -W 1300 -s %f", scaleStart);

            //fork
            pid = fork();

            //make sure pid not negative
            if (pid >= 0){
                //check if parent
                if (pid == 0){
                    
                    int exe = execvp(command[0], command);
                    //check for error
					if(exe == -1) {
						printf("mandel: command error: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}
                }
            }
            //fork error check
            else {
				perror("Failed to fork");
				exit(1);
			}

            //adjust scale
            scaleStart = scaleStart*zoom;

            free(command[6]);
            free(command[8]);

        }
        //ending processes
        else{
            pid_t w = wait(&status);
            //error check wait
            if(w == -1) {
				printf("wait error: %s\n", strerror(errno));
				exit(1);			
			}
            //successful wait
            else{
                //decrementing forks
                numFork--;
            }
        }
        

    }

    return 0;
}
