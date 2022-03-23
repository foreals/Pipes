#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

#define BUFFER_SIZE BUFSIZ

char *reverse(char *str);

int main(){

  int fd[2];
  pid_t pid1;

  if (pipe(fd)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
  
  pid1 = fork();

  if (pid1 < 0)
    {
      fprintf(stderr, "fork Failed" );
      return 1;
    }

  if (pid1 > 0) {
    //PARENT PROCESS
    char input[BUFFER_SIZE];
    close(fd[0]);

    //READ INPUT ND WRITE TO PIPE
    while (strcmp(input, "quit\n") != 0){
      fgets(input, BUFSIZ, stdin);
      write(fd[1], input, strlen(input)+1);
    }
    //CLOSE PIPE READ AND WRITE
    close(fd[1]);
    close(fd[0]);
    
  } else {
    //CHILD PROCESS
    char input[BUFFER_SIZE];
    close(fd[1]);

    //READ INPUT AND REVERSE
    while (read(fd[0], &input, BUFFER_SIZE) > 0) 
    {
      reverse(input);
      printf("\nReverse: %s\n\n", input);
    }
    close(fd[0]);
    exit(0);
  }
  
  return 0;
}

char *reverse(char *str){
  char c, *front, *back;
  
  if(!str || !*str)
    return str;
  for(front=str,back=str+strlen(str)-1;front < back;front++,back--){
    c=*front;*front=*back;*back=c;
  }
  return str;
}
