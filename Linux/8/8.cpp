#include<stdio.h>
#include<time.h>
#include<linux/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>

bool check(char *ch, char c){
	int i=0;
	while (ch[i]!='\0'){
		if (ch[i]==c) return true;
		i++;
	}
	return false;
}

void list(char *name,char *ch){
	DIR *dirname;
	struct dirent *content;
	struct stat sb;
	struct tm *ctime;
	int i;
	if ((dirname=opendir(name))==NULL){
		printf("This directory do not exist\n");
		return;
	}
	chdir(name);
	while ((content=readdir(dirname))!=NULL){
		if ((content->d_name[0]=='.')&&(check(ch,'a')==false)){}
		else{
			if (check(ch,'i')==true){
				printf("%6ld  ",(long)content->d_ino);
			}
			if (check(ch,'l')==true){
				stat(content->d_name,&sb);
				ctime = gmtime(&sb.st_mtime);
				// to check the type of the file
				
				if (content->d_type==4) printf("d");
				else if (content->d_type==10) 	printf("l");
				else if (content->d_type==1)	printf("p");
				else printf("-");
				// print out the access permission of the file
				if (sb.st_mode&S_IRUSR) printf("r");else printf("-");
				if (sb.st_mode&S_IWUSR) printf("w");else printf("-");
				if (sb.st_mode&S_IXUSR) printf("x");else printf("-");
				if (sb.st_mode&S_IRGRP) printf("r");else printf("-");
				if (sb.st_mode&S_IWGRP) printf("w");else printf("-");
				if (sb.st_mode&S_IXGRP) printf("x");else printf("-");
				if (sb.st_mode&S_IROTH) printf("r");else printf("-");
				if (sb.st_mode&S_IWOTH) printf("w");else printf("-");
				if (sb.st_mode&S_IXOTH) printf("x");else printf("-");
				// print out the link number of the file
				printf("%3d",sb.st_nlink);
				// print out the user and group of the file
				struct passwd *passwd;
				struct group  *group;
				passwd = getpwuid(sb.st_uid);
				group =  getgrgid(sb.st_gid);
				printf("%6s",passwd->pw_name);
				printf("%6s",group->gr_name);

				//print out the size of the file
				printf("%6d  ",(int)(sb.st_size));
				
				//print out the last modified time
				printf("%d month %3d %02d:%02d ",1+ctime->tm_mon,ctime->tm_mday,ctime->tm_hour,ctime->tm_min);
				
				//print out the name of the file
				printf("%s\n",content->d_name);
				//printf("\033[NULL;42m%s\033[5m\n",content->d_name);
			}
			else printf("%s\t",content->d_name);
		}
	}
	closedir(dirname);
	if (check(ch,'l')==false) printf("\n");
	//chdir("..");
}

int main(int argc, char *argv[])
{
	char *name = (char *)"./";
	//name = "./";
	if (argc==1)	list(name,(char *)" ");
	else if (argc==2) list(name,argv[1]);
	return 0;
}