//this my UBERSHELL my own implementation of a shell in C aimed 
// for unix like operating systems, it is a work in
// progress and currently supports basic command execution and some built-in
//was added to the specificati was added to the 
//specific commands like cd and exit.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "builtin.h"

void shl_loop(void);
char *shl_read_line(void);
char **shl_parse_line(char *line);
int shl_launch(char **args);
int shl_execute(char **args);

#define SUCCESS 1
#define ERROR -1
#define SHL_RL_BFSIZE 1024
#define SHL_TOK_BUFSIZE 64
#define SHL_TOK_DELIM " \t\r\n\a"


//initialize -> interpret -> terminate
int main(int argc, char *argv[]) {

        shl_loop();


        return SUCCESS;

}

//read command -> parse -> execute
void shl_loop(void){
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = shl_read_line();
        args = shl_parse_line(line);
        status = shl_execute(args);

        free(line);
        free(args);
    } while(status);
}


//read line from stdinput
char *shl_read_line(void){
    char *line = NULL;
    int position = 0;
    char* buffer = malloc(sizeof(char)*SHL_RL_BFSIZE);
    int c;
    int bufsize = SHL_RL_BFSIZE;

    if(!buffer){
        fprintf(stderr,"shl: allocation feild\n");
        exit(ERROR);
    }

    while(1){
        
        c = getchar();

        if(c==EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        //reallocate if exceeded buffer
        if(position>=SHL_RL_BFSIZE){
           bufsize += SHL_RL_BFSIZE;
           buffer = realloc(buffer,bufsize);
           if(!buffer){
            fprintf(stderr,"shl : reallocation error \n");
            exit(ERROR);
           }
        }
    }


}


//parse a line into arguements of tokens

char **shl_parse_line(char *line){
    int bufsize = SHL_TOK_BUFSIZE;
    int pos = 0;
    char **tokens = malloc(bufsize*sizeof(char*));
    char *token;

    if(!tokens){
        fprintf(stderr, "shl : allocation error\n");
        exit(ERROR);
    }

    //split the line using delims
    token = strtok(line,SHL_TOK_DELIM);

    while(token != NULL){
        tokens[pos] = token;
        pos++;
        if(pos>=bufsize){
            bufsize +=SHL_TOK_BUFSIZE;
            tokens = realloc(tokens,bufsize*sizeof(char*));
            if(!tokens){
                fprintf(stderr, "shl: allocation error\n");
                exit(ERROR);
            }
        }
        token = strtok(NULL,SHL_TOK_DELIM);
    }
    tokens[pos]=NULL;
    return tokens;
}

int shl_launch(char **args){
    pid_t pid;
    pid_t wpid;
    int status;

    pid = fork();
    if(pid==0){
        if(execvp(args[0],args)==-1){
            perror("shl");
        }
        exit(ERROR);
    } else if (pid<0){
        //forking error
        perror("shl");
    }else{
        //parent proc
        do{
            wpid = waitpid(pid,&status,WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return SUCCESS;
}


//execute commands
int shl_execute(char **args){
    int i;

    if (args[0] == NULL){
        return 1;
    }

    for(i=0; i < shl_num_builtins(); i++){
        if(strcmp(args[0],builtin_str[i])==0){
            return (*builtin_func[i])(args);
        }
    }
    //if its not builtin then launch process
    return shl_launch(args);
}