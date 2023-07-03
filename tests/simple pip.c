/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple pip.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 04:45:55 by wayden            #+#    #+#             */
/*   Updated: 2023/06/30 13:14:42 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // open, close, read, write, access, dup, dup2, execve, fork, pipe, unlink
#include <fcntl.h>    // open
#include <stdlib.h>   // malloc, free
#include <stdio.h>    // perror
#include <string.h>   // strerror
#include <sys/types.h>  // wait, waitpid
#include <sys/wait.h>  // wait, waitpid

int main(int argc, char *argv[]) 
{
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s cmd1 cmd2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) 
    {
        // Processus fils
        dup2(pipefd[1], STDOUT_FILENO);  // Redirige la sortie standard vers l'extrémité d'écriture du pipe
        close(pipefd[0]);  // Ferme l'extrémité de lecture inutilisée du pipe
        close(pipefd[1]);  // Ferme l'extrémité d'écriture du pipe

        char *command[] = {"sh", "-c", argv[1], NULL};
        execve("/bin/sh", command, NULL);

        perror("execve");
        exit(EXIT_FAILURE);
    } 
    else 
    {
        // Processus parent
        wait(NULL);  // Attend la fin de l'exécution du processus fils

        dup2(pipefd[0], STDIN_FILENO);  // Redirige l'entrée standard vers l'extrémité de lecture du pipe
        close(pipefd[0]);  // Ferme l'extrémité de lecture du pipe
        close(pipefd[1]);  // Ferme l'extrémité d'écriture inutilisée du pipe

        char *command[] = {"sh", "-c", argv[2], NULL};
        execve("/bin/sh", command, NULL);

        perror("execve");
        exit(EXIT_FAILURE);
    }

    return 0;
}