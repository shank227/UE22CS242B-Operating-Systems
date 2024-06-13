#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#define MAX 100 

int main(){ 
    char str[MAX]; 
    int pipefd[2]; 
    pid_t pid; 
    if (pipe(pipefd)== -1){ 
        perror("pipe"); 
        exit(EXIT_FAILURE); 
    } 
    
    pid = fork(); 
    if (pid == -1){ 
        perror("fork"); 
        exit(EXIT_FAILURE); 
    } 
    
    if(pid==0){ 
        close(pipefd[1]); 
        char reversed_str[MAX]; 
        int n = read(pipefd[0],str,MAX); 
        for(int i=n-1,j=0;i>=0;i--,j++){ 
            reversed_str[j]=str[i]; 
        } 
        reversed_str[n]='\0'; 
        printf("Reversed string : %s\n",reversed_str); 
        close(pipefd[0]);
    } 
        
    else{ 
        close(pipefd[0]); 
        printf("Enter string: "); 
        fgets(str,MAX,stdin); 
        int n = strlen(str); 
        if(n>0 && str[n-1]=='\n'){ 
            str[n-1]='\0'; n--; 
        } 
        
        write(pipefd[1],str,n); 
        close(pipefd[1]); 
    }

    return 0; 
}
