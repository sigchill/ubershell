#ifndef BUILTIN_H
#define BUILTIN_H

//declaration for builtin shell commands
int shl_cd(char **args);
int shl_help(char **args);
int shl_exit(char **args);
int shl_pwd(char **args);
int shl_echo(char **args);
int shl_clear(char **args);
int shl_env(char **args);

int shl_num_builtins();
extern char *builtin_str[];
extern int (*builtin_func[])(char **);

#endif