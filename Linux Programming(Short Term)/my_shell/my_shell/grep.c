/*
 * filename: grep.c
 * owner: Leon Zhou
 * Email: the.reason.sake@gmail.com
 * description:
 *      a simple version to inplement the Linux command 'grep'
 */

#include<sys/types.h>
#include<regex.h> //to use the regular expression
#include<stdio.h>
#include<string.h>


int main(int argc, char *argv[])
{

    FILE *fp;

    int x, z, cflags = 0, lno = 0;
    char ebuf[128], lbuf[256]; //error buf and line buf
    regex_t reg;
    char *pattern;
    regmatch_t pm[10]; // at most match 10 patterns
    const size_t nmatch = 10;


    if(argc != 3) // the command is 'grep pattern filename'
        printf("parameter error!");
    else
    {
        pattern = argv[1];
        z = regcomp(&reg, pattern, cflags); //compile the pattern

        if(z != 0)
        {
            regerror(z, &reg, ebuf, sizeof(ebuf));
            fprintf(stderr, "%s: pattern '%s'\n", ebuf, pattern);
            return 1;
        }

        if((fp = fopen(argv[2], "rb")) == NULL){
            fprintf(stderr, "open '%s' error", argv[2]);
            return 1;
        }

        while(fgets(lbuf,sizeof(lbuf), fp))
        {
            ++lno;
            if((z=strlen(lbuf)) > 0 && lbuf[z-1]=='\n')
                lbuf[z-1] = '\0';

            z = regexec(&reg, lbuf, nmatch, pm, 0); // match the pattern
            if(z == REG_NOMATCH)
                continue;
            else if(z != 0)
            {
                regerror(z, &reg, ebuf, sizeof(ebuf));
                fprintf(stderr, "%s: regcom('%s')\n", ebuf, lbuf);
                return 2;
            }

            for(x = 0; x < nmatch && pm[x].rm_so != -1; x++)
            {
                char sub[256];
                if(!x)
                    //print the line containing the pattern
                    printf("%04d: %s\n", lno, lbuf); 
            }
        }

        regfree(&reg); // delete the regular express

    }

    return 0;
}
