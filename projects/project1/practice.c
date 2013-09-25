#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    
    int X = open("./output_file.txt", O_WRONLY);
	
    pid_t pid;
    
	int status = 0;
	
    int count = 0;
    int parentCount = 0;
    
    printf("before the fork\n");
    
    

    pid = fork();
	
    if(pid == 0){
        printf("this is the child process\n");
        
	        printf("child pid: %d\n",pid);
		
		printf("parent's pid: %d\n",getppid());

		execl("/bin/ls", "-l", (char*)0);
		
		

		while (count < 10) {
			count++;
			sleep(1);
			printf("Counting: %d\n", count);
		}
		
		
		
		//printf("\nswag\n");
		
		//exit(status);
    }else if(pid > 0 ){
      //	printf("parent pid: %d\n",pid);
		
		//wait(&status);
        printf("the parent process\n");
        while(parentCount < 10){
			printf("Parent proccess: %d\n", parentCount);  
			sleep(1);
			parentCount++;
        } 
		
    }
    
	close(X);
    
    return 0;   
}
