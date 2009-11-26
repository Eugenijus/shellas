/*
 * main.c
 *
 *  Created on: Nov 9, 2009
 *      Author: eugenijus
 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_ELEMENTS 100
#define MAX_ELEM_SIZE 50

/*
 * isimti stringa iki tarpo
 * ideti i arg masyva
 * pirmai f paduodam ptr i stringa f1(p1 +4 +4)
 * f2
 * p1 = "asd asd cat"
*/
int praleiskTarpa(char *str){
	int i=0;
	while(str[i]==' ' && str[i]!='\0'){
		i++;
	}
	return i;
}

char * skaitykIkiTarpo(char *str){
	int j = 0;
	int k = strlen(str);
	char *param=malloc(MAX_ELEM_SIZE); //stringas
	if(k == 0){
		return (char *)NULL;
	}
	//printf("%c\n",str[0]);

	for(;j<k;j++){
		if(str[j] != ' '){
			param[j] = str[j];
			param[j+1] = '\0';
		}
		else{
			return param;
		}
	}
	return param;
}

//returns array of strings
char ** parseString(char *line){
	int j = 0;
	int offset = 0;
	char **args= (char **)malloc(sizeof(char *) * MAX_ELEMENTS); //100 string'u masyvas
	//printf("%s\n", line);
	//printf("%d\n", strlen(line));

	while((args[j] = skaitykIkiTarpo(line+offset)) != NULL){
		offset += strlen(args[j]);
		offset += praleiskTarpa(line+offset);
		//printf("%s\n",args[j]);
		j++;
	}
	return args;
}
void freeArgs(char **args){
	int j = 0;
	for(; (args[j]!=NULL) ;j++){
		//printf("%s\n",args[j]);
		free(args[j]);
	}
	free(args);
}

int isRedirect(char *str){
	if (strcmp(">",str)==0){
		return 1; //stdout
	}
	if (strcmp("<",str)==0){
		return 0; //stdin
	}
	if (strcmp(">>",str)==0){
		return 1; //stdout + append
	}
	if (strcmp("<<",str)==0){
		return 0; //stdin + append
	}
	if (strcmp("2>",str)==0){
		return 2; //stderr
	}
	if (strcmp("2>>",str)==0){
		return 2; //stderr + append
	}
	return -1;
}

char ** stripRedirects(char ** args){
	char **striped_args= (char **)malloc(sizeof(char *) * MAX_ELEMENTS);
	int j,i = 0;
	//int offset = 0;
	for(;(args[i]!=NULL) ;i++){
		j = isRedirect(args[i]);
		switch(j){
		case 0: break;
		case 1: break;
		case 2: break;
		case -1: break;
		default: break;
		}
	}
	free(args);
	return striped_args;
}

// execvp(argv[0], argv)

int main(){
	int status;
	char c = '\0';
	char line[255]; //char line[] = char *line tik reik malloc
	int i = 0;
	char **commands = NULL;
	printf("[Shellas] ");
	while(c != EOF) {
		c = getchar();
		if(c == '\n'){
			line[i]='\0';

			commands = parseString(line);
			if (strcmp("exit",commands[0])==0){
				freeArgs(commands);
				return 0;
			}
			if(fork()==0){
				//commands = stripRedirects(commands);
				execvp(commands[0],commands);
				perror("Error");
				return -1;
			}
			else{
				wait(&status);
				freeArgs(commands);
			}
			printf("[Shellas] ");
			i = 0;
			line[0] = '\0';
		}
		else {
			line[i] = c;
			i++;
		}
	}
	printf("\n");
	/*
	char *commands[10];
	char command1[] = "ls";
	char command2[] = "-l";
	commands[0]= command1;
	commands[1]= command2;
	commands[2]= '\0';
	//printf ("%s\n",commands[0]);
	//printf ("%s\n",commands[1]);
	if(fork()==0){
		execvp(commands[0],commands);
	}
	else{
		wait();
	}*/

	return 0;
}
