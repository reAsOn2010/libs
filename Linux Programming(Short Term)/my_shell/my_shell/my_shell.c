/*
 * filename: my_shell.c
 * owner: Leon Zhou
 * Email: the.reason.sake@gmail.com
 * description:
 *      a simple shell Interpreter
 *      function: cd, pwd, ls(simple version),
 *                grep(simple version),
 *                find(GNU version)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

char parse(char *cmd, char ** parsed_cmd);

int main(int argc, char *argv[])
{
    char cmd[1024]; //store the command
    char current_dir[1024]; //store the path of current work directory
    char file_dir[1024]; //store the path of 'my_shell' program
    char cmd_dir[1024]; //store the path of different command program
    char print_at_front[2048]; 
    char *parsed_cmd[20]; //store the parsed command (just like char *argv[])
    char choice = '\0';
    pid_t pid; //for outer command

    //initialize some constants
    memset(current_dir, 0, 1024);
    memset(file_dir, 0, 1024);
    getcwd(file_dir,sizeof(file_dir));
    strcat(file_dir,"/bin/");

    while(1){
        getcwd(current_dir, sizeof(current_dir));
        memset(cmd, 0, 1024); 
        print_at_front[0] = '\0';
        strcat(print_at_front, current_dir);
        strcat(print_at_front, " >>");

        printf("%s", print_at_front);

        fgets(cmd, 1024, stdin); //get the new command
        cmd[strlen(cmd) - 1] = '\0'; //repalce the '\n' with '\0'

        choice = parse(cmd, parsed_cmd); //call the parse function

        if(choice == 'Q') //logout
            break;

        switch(choice){
            case 'c': // cd
                if(chdir(parsed_cmd[1]) != 0)
                    printf("No such directory!\n");
                break;
            case 'l': //ls
                cmd_dir[0] = '\0';
                strcat(cmd_dir, file_dir);
                //create new process
                pid = fork();
                if(pid == 0){
                    execv(strcat(cmd_dir, "ls"), parsed_cmd);
                }
                else{
                    wait(NULL);
                }
                //end of the 'ls' process

                break;
            case 'p': // pwd
                printf("%s\n", current_dir);
            case 'g':
                cmd_dir[0] = '\0';
                strcat(cmd_dir, file_dir);
                //create new process
                pid = fork();
                if(pid == 0){
                    execv(strcat(cmd_dir, "grep"), parsed_cmd);
                }
                else{
                    wait(NULL);
                }
                //end of the 'grep' process

                break;
            case 'f':
                cmd_dir[0] = '\0';
                strcat(cmd_dir, file_dir);
                //create new process
                pid = fork();
                if(pid == 0){
                    execv(strcat(cmd_dir, "find"), parsed_cmd);
                }
                else{
                    wait(NULL);
                }
                //end of the 'find' process

                break;
            default:
                printf("No such command!\n");
        }
    }
}

char parse(char *cmd, char ** parsed_cmd)
{
    /* parse the command into string array */
    int i = 0;
    memset(parsed_cmd, 0, 20*sizeof(char *));
    parsed_cmd[i] = strtok(cmd, " ");
    while(parsed_cmd[++i] = strtok(NULL, " "));

    //choose the command
    if(strcmp(parsed_cmd[0], "logout") == 0) //logout
        return 'Q';
    else if(strcmp(parsed_cmd[0], "cd") == 0) //cd
        return 'c';
    else if(strcmp(parsed_cmd[0], "logout") == 0) //logout
        return 'Q';
    else if(strcmp(parsed_cmd[0], "ls") == 0) //ls
        return 'l';
    else if(strcmp(parsed_cmd[0], "pwd") == 0) //pwd
        return 'p';
    else if(strcmp(parsed_cmd[0], "grep") == 0) //grep
        return 'g';
    else if(strcmp(parsed_cmd[0], "find") == 0) //find
        return 'f';
    else
        return 'F';
}

