/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"


void gestionInOut(char *name, int *fd, char type);
void executeCmd(char **cmd);

int main()
{
	int stop = 0;

	while (!stop) {
		struct cmdline *l;
		int i, j;

		printf("shell> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);

			for (j=0; cmd[j]!=0; j++) {
				printf("%s ", cmd[j]);
				if(strcmp("quit", cmd[j]) == 0) {
					stop = 1;
					break;
				}
			}
			printf("\n");

			if(!stop){
				// Création de processus.
				if(l->seq[i+1] != 0){
					// pipe si deuxiéme commande existe
					int fd[2];
					pipe(fd);

					pid_t p = Fork();
					if(p == -1){ fprintf(stderr, "Erreur de creation de processus"); exit(-3);}
					if(p > 0){

						pid_t p1 = Fork();

						if(p1 == 0){
							close(fd[0]);

							if(l->in){
								gestionInOut(l->in, &(fd[1]), 'i');
							}

							if(l->out)
								gestionInOut(l->out, &(fd[1]), 'i');

							executeCmd(cmd);
						} else if(p1 < 0){
							exit(-3);
						}

						wait(NULL); // le parent attends son fils
					} else {
						// redirections
						char **cmd2 = l->seq[i +1];
						close(fd[1]);
						if(l->out)
							gestionInOut(l->out, &(fd[0]), 'i');

						if(l->in)
							gestionInOut(l->in, &(fd[0]), 'i');

						executeCmd(cmd2);
					}
					i++;




				} else {
					pid_t p = Fork();
					if(p == -1){ fprintf(stderr, "Erreur de creation de processus"); exit(-3);}
					if(p > 0){
						wait(NULL); // le parent attends son fils
					} else {
						// redirections
						int fd;
						if(l->in){
							gestionInOut(l->in, &fd, 'i');
						}

						if(l->out){
							gestionInOut(l->out, &fd, 'o');
						}

						executeCmd(cmd);
					}
				}

			} else{
				fprintf(stdout, "Arret...\n");
			}

		}
	}

	return 0;
}


void gestionInOut(char *name, int *fd, char type){
	
	if(type == 'o') 
		*fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644); // sortie
	else if(type == 'i')
		*fd = open(name, O_RDONLY); // entrée
	else{
		fprintf(stderr, "type d'entree inconnu\n");
		exit(-1);
	}

	switch(errno){
		case ENOENT:
			fprintf(stderr, "command not found\n");
			exit(EXIT_FAILURE);
		case EACCES:
			fprintf(stderr, "%s: Permission denied.\n", name);
			exit(EXIT_FAILURE);
		default:
			perror("execv");
			exit(EXIT_FAILURE);		
	}


	if(type == 'o')
		dup2(*fd, STDOUT_FILENO);
	else
		dup2(*fd, STDIN_FILENO);

	close(*fd);
}


void executeCmd(char **cmd){
	char path[256];
	// strcpy(path, "/bin/");
	strcpy(path, "");

	strcat(path, cmd[0]);
	execvp(path, cmd);

	perror("execv");
	exit(EXIT_FAILURE);
}