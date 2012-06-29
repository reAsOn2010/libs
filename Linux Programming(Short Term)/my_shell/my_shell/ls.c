/*
 * filename: ls.c
 * owner: Leon Zhou
 * Email: the.reason.sake@gmail.com
 * description:
 *      a simple version to inplement the Linux command 'ls'
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *ptr;
    int i = 0;
    char current_dir[1024]; //store the current directory
    getcwd(current_dir, sizeof(current_dir));
    //printf("%s\n", current_dir);
    if(argc == 1) // the command is just 'ls'
    {
        dir = opendir(current_dir);
        while((ptr=readdir(dir)) != NULL)
        {
            printf("%s\t",ptr->d_name);
        }
        printf("\n");
    }

    else if(argc == 2)
    {
        if(strcmp(argv[1], "-l") == 0) // the command is 'ls -l'
        {
            dir = opendir(current_dir);
            printf("filename\t\tlength\t\t\ttype\t\t\tfile no\n");
            while((ptr=readdir(dir)) != NULL)
            {
                printf("%s\t\t\t",ptr->d_name);
                printf("%d\t\t\t", ptr->d_reclen);
                printf("%d\t\t\t", ptr->d_type);
                printf("%d\t\t\t", ptr->d_fileno);
                printf("\n");
            }
            printf("\n");
        }
        else{ // the command is 'ls /path/to/some/directory'
            dir = opendir(argv[1]);
            while((ptr=readdir(dir)) != NULL)
            {
                printf("%s\t",ptr->d_name);
            }
            printf("\n");
        }
    }
    else if(argc == 3){ // the command is 'ls -l /path/to/some/directory'
        if(strcmp(argv[1], "-l") == 0)
        {
            dir = opendir(argv[2]);
            printf("filename\t\tlength\t\t\ttype\t\t\tfile no\n");
            while((ptr=readdir(dir)) != NULL)
            {
                printf("%s\t\t\t",ptr->d_name);
                printf("%d\t\t\t", ptr->d_reclen);
                printf("%d\t\t\t", ptr->d_type);
                printf("%d\t\t\t", ptr->d_fileno);
                printf("\n");
            }
            printf("\n");
        }
        else {
            perror("undefined parameter!\n");
            exit(1);
        }
    }
    else{
        perror("Parameter error!");
        exit(1);
    }
    return 0;
}
