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
		/* i est le Compteur du nombre de commande */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);

			for (j=0; cmd[j]!=0; j++) {
				
				printf("%s ", cmd[j]);
			}
			printf("\n");
		}

		printf("i: %d\n", i);
		

		// Création de processus.
		if (i > 1){ // plus d'un processus
			// Création des pipes nécessaires
			int fd[i-1][2];
			pid_t pids[i];

			// Ouverture des pipes
			for (int k = 0; k < i - 1; k++)
			{
				pipe(fd[k]);
			}

			// Création des fils
			for(int k = 0; k < i; k++){
				char **cmd = l->seq[k];

				
				// Fils k;
				
				pid_t p = Fork();
				if(p == -1){ fprintf(stderr, "Erreur de creation de processus"); exit(-3);}

				pids[k] = p; // On conserve les pids au cas où.
				
				if(p == 0){

					if(k > 0) // premiere commande non pris en compte
						dup2(fd[k - 1][0], STDIN_FILENO);
					
					if(k < i -1) // derniere commande non pris en compte
						dup2(fd[k][1], STDOUT_FILENO);
					
					// Fermeture des pipes par le fils
					for (j = 0; j < i - 1; j++) {
						close(fd[j][0]);
						close(fd[j][1]);
					}

					int fd_redirection;
					if(l->in){
						gestionInOut(l->in, &fd_redirection, 'i');
					}

					if(l->out){
						gestionInOut(l->out, &fd_redirection, 'o');
					}
					executeCmd(cmd);
				}

			
			}

			// Fermeture des pipes par le père
			for (j = 0; j < i -1; j++) {
				close(fd[j][0]);
				close(fd[j][1]);
			}

			// attente des fils
			for(int k = 0; k < i; k++){
				waitpid(pids[k], NULL, 0);
			}
			
		} else if (i == 1 && strcmp("quit", l->seq[0][0]) != 0) { // un seul processus
			char **cmd = l->seq[0];
			
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
		} else if(strcmp("quit", l->seq[0][0]) == 0){
			stop = 1; // cas de quit dans pipe pas gerer pour l'instant
		}


		if(stop){
			fprintf(stdout, "Arret...\n");
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

	if(*fd < 0){
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

	if (errno == ENOENT) {
		fprintf(stderr, "%s: command not found\n", path);
	}
	else if (errno == EACCES) {
		fprintf(stderr, "%s: Permission denied\n", path);
	}
	else {
		perror("execvp");
	}
	exit(EXIT_FAILURE);
}