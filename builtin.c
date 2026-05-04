#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"

extern char **environ;

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "pwd",
    "echo",
    "clear",
    "env"
};


int (*builtin_func[]) (char **) = {
    &shl_cd,
    &shl_help,
    &shl_exit,
    &shl_pwd,
    &shl_echo,
    &shl_clear,
    &shl_env
};

//number of funcitons available
int shl_num_builtins(){
    return sizeof(builtin_str)/sizeof(char*);
}


//change directory
int shl_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr , "shl: arguement expected for \"cd\"\n");
    }
    else {
        if(chdir(args[1])!=0){
            perror("shl");
        }
    }
    return 1;

}

//help command
int shl_help(char **args){
    int i;
    printf("UBERSHELL\n");
    printf("Type commands and arguements, press ENTER\n");
    printf("These commands are builtin into the shell\n");

    for(i=0; i<shl_num_builtins(); i++){
        printf("   %s\n",builtin_str[i]);
    }

    printf("For more info use the man command\n");
    return 1;
}

//exit the shell
int shl_exit(char **args){
    return 0;
}

//get pwd
int shl_pwd(char **args){
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s\n", cwd);
    } else {
        perror("shl");
    }
    return 1;
}

//echo command
int shl_echo(char **args){
    int i = 1;
    while(args[i] != NULL){
        printf("%s", args[i]);
        if(args[i+1] != NULL) printf(" ");
        i++;
    }
    printf("\n");
    return 1;
}

//clear the terminal
int shl_clear(char **args){
    printf("\033[H\033[J");
    return 1;
}

//show enviromental variables
int shl_env(char **args){
    int i;
    for(i = 0; environ[i] != NULL; i++){
        printf("%s\n", environ[i]);
    }
    return 1;
}