#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //to fix strerror warning


#define EPERM            1      /* Operation not permitted */
#define ENOENT           2      /* No such file or directory */
#define ESRCH            3      /* No such process */
#define EINTR            4      /* Interrupted system call */


void display_message( int s ){
	printf("copyit: still copying...\n");
    alarm(1);
    signal(SIGALRM,display_message);
}

int main(int argc, char *argv[]){
    //read from standard input
    if (argc < 3){
        printf("Not enough arguments");
    }
    else if (argc > 3){
        printf("Too many arguments");
    }
    else{
        char *SourceFile = argv[1];
        char *TargetFile = argv[2];

        //set up the periodic message
        signal(SIGALRM,display_message);
        alarm(1);
        //open the source file or exit with an error
        int fd = open(SourceFile,O_RDONLY,0);
        if(fd<0) {
	        printf("Unable to open %s: %s\n",SourceFile,strerror(errno));
	        exit(1);
        }

        //create the target file or exit with an error
        int CR = creat(TargetFile, S_IRUSR | S_IWUSR);
        if (CR<0){
            printf("Unable to create %s: %s\n",TargetFile,strerror(errno));
	        exit(1);
        }
        int sz = 10;
        //loop over {
        while(sz == 10){
            char *F = (char *) calloc(100, sizeof(char));
            //read a bit of data from the source file
            
            sz = read(fd, F, 10);
            //if the read was interrupted, try it again
            while (errno == EINTR){
                sz = read(fd,F,10);
            }
            //if there was an error reading, exit with an error
            if (sz<0){
                printf("Unable to read %s: %s\n",SourceFile,strerror(errno));
	            exit(1);
            }
            //if no data left, end the loop
            if (sz == 0){
                break;
            }
            //write a bit of data to the target file
            int W = write(CR,F,sz);
            // if the write was interrupted, try it again
            while(errno == EINTR){
                W = write(CR,F,10); 
            }
            //if not all the data was written, exit with an error
            if (W<0){
                printf("Unable to read %s: %s\n",SourceFile,strerror(errno));
	            exit(1);
            }
        }
        //close both files
        close(fd);
        close(CR);
        //print success message
        printf("It is finally over! You have succeeded!!!");
    }

        
}