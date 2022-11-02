#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <dirent.h> 
#include <time.h>
#include <sys/sysmacros.h>

#define MAX_PATH 100
#define MAXARG 7

typedef struct{
	char  data[100];
}History;

typedef struct{
	char alias[100];
	char command[100];	
}Alias;

int print_prompt(){
	char *buffer = (char *)malloc(MAX_PATH);
	char *cwd = (char *)getcwd(buffer, MAX_PATH);
	
	if(cwd != 0){
		fprintf(stdout, "%s > myshell: ", buffer);
		free(buffer);

		return 0;
	}else{
		printf("cwd 0\n");
	}

}

void my_cat(char* arg[]){
	char ch;
	int fd;

	printf("<%s>\n", arg[0]);
	if((fd = open(arg[0], O_RDONLY))<0){
		printf("file open error\n");
		
	}else{		
		fd = open(arg[0], O_RDONLY); // file open

		while(read(fd, &ch, 1)){ // file read&write
			write(1, &ch, 1);
		}

		close(fd);
	}
}

void my_pwd(){
	char buf[100];
	getcwd(buf, 100);
	printf("%s\n", buf);
}

void my_cd(char* arg[]){
	char buf[100];

	if(chdir(arg[0]) == 0){
		getcwd(buf,100);
		printf("path: %s\n", buf);
	}else{
		printf("path error\n");
	}
}

void my_mkdir(char* arg[]){	
	mkdir(arg[0], 0755);
}

void my_ls(char* arg[]){
	DIR *dir_info;
	struct dirent *dir_entry;
	char* cwd = (char*)malloc(sizeof(char)*1024);
	
	getcwd(cwd,1024);

	if(arg[0] != NULL){
		strcat(cwd,"/");	
		strcat(cwd, arg[0]);
	}

	dir_info = opendir(cwd); // open current dir
	if(dir_info != NULL){
		while(dir_entry = readdir(dir_info)){
			if(dir_entry->d_name[0]!='.'){	
				printf("%s\n", dir_entry->d_name);
			}
		}
		closedir(dir_info);
		free(cwd);
	}else{
		printf("error: dir_info == NULL\n");
	}
}



void my_ps(){
	DIR *dir_info;
	struct dirent *dir_entry;
	struct stat file_stat;

	int pid;
	char *s, *save;
	char cmdLine[256];
	char *cmd[256];
	
	char cmdLineU[512];
	char *cmdU[256];

	int uid = getuid();	
	

	printf(" PID     CMD\n");
	
	dir_info = opendir("/proc");
	while((dir_entry = readdir(dir_info))!=NULL){
		lstat(dir_entry->d_name, &file_stat);
		
		if(!S_ISDIR(file_stat.st_mode)){ // checking dir
			continue;
		}
		
		pid = atoi(dir_entry->d_name); 
	
		if(pid<=0){
			continue;
		}

		char path[256]="/proc/";
		strcat(path, dir_entry->d_name);
		strcat(path, "/status");
	
		char tempPath[256]="/proc/";
		strcat(tempPath, dir_entry->d_name);
		strcat(tempPath, "/stat");
		
		FILE *file1;
		file1 = fopen(path, "r"); // /proc/pid/status

		FILE *file;
		file = fopen(tempPath, "r"); // proc/pid/stat

		memset(cmdLineU, 0, sizeof(cmdLineU)); 
		memset(cmdLine, 0, sizeof(cmdLine));

		int lineNum = 0;	
		
		while(fgets(cmdLineU, 256, file1)!= NULL){ // file 1 line
			lineNum++;
			if(lineNum == 9){ // line9 uid 1000 1000 1000 1000
				int cmdUNum = 0;

				s = strtok_r(cmdLineU, " \t", &save);
				while(s){
					cmdU[cmdUNum++] = s;
					s = strtok_r(NULL, " \t", &save);
				}
				if(uid == atoi(cmdU[1])){ // checking useruid == proc uid
					fgets(cmdLine, 256, file);
					int cmdNum = 0;
					s = strtok_r(cmdLine, "( )", &save);
					while(s){
						cmd[cmdNum++] = s;
						s = strtok_r(NULL, "( )", &save);
					}
					printf("[%4d]   %s\n", pid, cmd[1]);
				}
			}
		}
		fclose(file1);
		fclose(file);	
	
	}
	closedir(dir_info);
}

void my_cp(char* arg1[], char* arg2[]){
	int fd1, fd2;
	char ch;

	fd1 = open(arg1[0], O_RDONLY);
	if(fd1< 0){
		printf("file open error\n");		
	}else{
		fd2 = open(arg2[0], O_WRONLY|O_CREAT|O_TRUNC, 0644);
		
		while(read(fd1, &ch, 1)){ // file read&write
			write(fd2, &ch, 1);
		}
		close(fd1);
		close(fd2);
	}	
}

void my_rm(char* arg[]){
	remove(arg[0]);
}

void my_ln(char* arg1[], char* arg2[]){
	link(arg1[0], arg2[0]);
}

void my_stat(char* arg[]){
	struct stat sb;
	
	if(lstat(arg[0], &sb) == -1){
		printf("lstat error\n");
	}else{
		printf("ID of containing device:   [%lx, %lx]\n", (long)major(sb.st_dev), (long)minor(sb.st_dev));
		printf("File type:                 ");
	
		switch(sb.st_mode & S_IFMT){
			case S_IFBLK: printf("block device\n"); break;
			case S_IFCHR: printf("character  device\n"); break;
			case S_IFDIR: printf("directory\n"); break;
			case S_IFIFO: printf("FIFO/pipe\n"); break;
			case S_IFLNK: printf("symlink\n"); break;
			case S_IFREG: printf("regular file\n"); break;
			case S_IFSOCK: printf("socket\n"); break;
			default: printf("unknown?");
		}

		printf("I-node number:             %ld\n", (long)sb.st_ino);
		printf("Mode:                      %lo(octal)\n", (unsigned long)sb.st_mode);
		printf("Link count:                %ld\n", (long)sb.st_nlink);
		printf("Ownership:                 UID=%ld   GID=%ld\n", (long)sb.st_uid, (long)sb.st_gid);
		printf("Preferred I/O block size:  %ld bytes\n", (long)sb.st_blksize);
		printf("File size:                 %lld bytes\n", (long long)sb.st_size);
		printf("Blocks allocated:          %lld \n", (long long)sb.st_blocks);
	
		printf("Last status change:        %s", ctime(&sb.st_ctime));
		printf("Last file access:          %s", ctime(&sb.st_atime));
		printf("Last file modification:    %s", ctime(&sb.st_mtime));
	}
}

void my_chmod(char* arg1[], char* arg2[]){
	if(access(arg2[0],F_OK)!=0){
		printf("%s is not existed\n", arg2[0]);
	}
	
	int mode=0;
	sscanf(arg1[0], "%o", &mode); // str -> octal
	
	if(chmod(arg2[0], mode)!=0){
		printf("fail change mode\n");
	}else{
		printf("success change mode\n");
	}
}


void input_alias(Alias *aliasList, char* ali[], char* command[], int ali_num){
	strcpy(aliasList[ali_num].alias, ali[0]);
	strcpy(aliasList[ali_num].command, command[0]);
}

void my_alias(Alias *aliasList, int i){
	printf("%d. alias %s='%s'\n", i, aliasList[i].alias, aliasList[i].command);
}

void input_history(History *history, char *buf, int his_num){
	strcpy(history[his_num].data, buf);
}

void my_history(History *history, int i){	
	printf("  %d  %s", i, history[i].data);
}



int main(){
	char buf[256];
	char *buf1[10];
	char *arg[MAXARG];
	char *s, *save;
	int argv;
	static const char delim[] = " \t\n";
	
	int his_num=1;
	char his_tmp[100]= "";
	History history[100];
	
	int ali_num=0, aNum;
	char *ali[10];
	char *a, *asave;
	Alias aliasList[100];
	
	while(1){	
		print_prompt();
		fgets(buf, sizeof buf, stdin);

		if(buf[0]=='!'){ // checking '!'
			strncpy(his_tmp, buf+1, strlen(buf)-1);
			int num = atoi(his_tmp);
			if(num<=0){
				printf("%sis not number\n", buf+1);
			}else{
				strcpy(buf, history[num*2].data);
			}
		}

		buf1[0] = buf; // input_history
		input_history(&history[his_num], buf1[0], his_num);
		his_num++;

		s = strtok_r(buf, delim, &save); // parsing
		argv = 0;
		while(s){
			arg[argv++] = s;
			s = strtok_r(NULL, delim, &save);
		}
		arg[argv] = (char *)0;
		
		for(int i=0; i<ali_num; i++){ // checking alias
			if(!strcmp(arg[0], aliasList[i*2].alias)){
				strcpy(arg[0], aliasList[i*2].command);
			}
		}	
			
		if(!strcmp(arg[0], "exit")){ // command start
			break;
		} else if(!strcmp(arg[0], "cat")){
			for(int i=1; i<argv; i++){
				my_cat(&arg[i]);	
			}
		} else if(!strcmp(arg[0], "pwd")){
			my_pwd();
		} else if(!strcmp(arg[0], "cd")){
			my_cd(&arg[1]);
		} else if(!strcmp(arg[0], "mkdir")){	
			my_mkdir(&arg[1]);
		} else if(!strcmp(arg[0], "ls")){
			my_ls(&arg[1]);
		} else if(!strcmp(arg[0], "history")){
			for(int i=1; i<his_num; i++){
				my_history(&history[i], i);
			}
		} else if(!strcmp(arg[0], "ps")){
			my_ps(&history);
		} else if(!strcmp(arg[0], "cp")){
			my_cp(&arg[1], &arg[2]);
		} else if(!strcmp(arg[0], "rm")){
			my_rm(&arg[1]);
		} else if(!strcmp(arg[0], "ln")){
			my_ln(&arg[1], &arg[2]);
		} else if(!strcmp(arg[0], "stat")){
			my_stat(&arg[1]);
		} else if(!strcmp(arg[0], "chmod")){
			my_chmod(&arg[1], &arg[2]);
	       	} else if(!strcmp(arg[0], "alias")){
			if(arg[1] == NULL){
				for(int i=0; i<ali_num; i++){
					my_alias(&aliasList[i], i);
				}
			}else{
				aNum = 0;
				
				a = strtok_r(arg[1], "='", &asave);
				while(a){
					ali[aNum++] = a;
					a = strtok_r(NULL, "='", &asave);
				}

				input_alias(&aliasList[ali_num], &ali[0], &ali[1], ali_num);
				ali_num++;
			}
		} else{
			printf("command %s is not found\n", arg[0]);
		}

		fflush(stdout);
	}


	return 0;
}
