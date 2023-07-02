/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 04:45:55 by wayden            #+#    #+#             */
/*   Updated: 2023/06/30 13:16:08 by wayden           ###   ########.fr       */
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
#define OUTPIPE pipe_fd[pid[1] - 1]



char **get_command(char *cmd)
{
    static char *full_cmd[] = {"sh", "-c", NULL, NULL};
    full_cmd[2] = cmd;
    return full_cmd;
}
//dernier des enfant premier a s'exec
void exec_first_process(char *infile, int outpipe[2])
{
    char *args[3];
    args[0] = "cat";
    args[1] = infile;
    args[2] = NULL;
    close(outpipe[0]);
    dup2(outpipe[1], STDOUT_FILENO);
    close(outpipe[1]);
    execve("/bin/cat", args, NULL);
    perror("Erreur lors de l'exécution de cat");
    exit(EXIT_FAILURE);
}

void exec_mid_process(int inpipe[2], int outpipe[2], char *cmd)
{
    char **comm;

    comm = get_command(cmd);
    close(inpipe[1]); //close write enfant pipe
    close(outpipe[0]);
    dup2(inpipe[0], STDIN_FILENO); // sortie enfant devient entrée standar
    close(inpipe[0]); //close read enfant pipe
    dup2(outpipe[1], STDOUT_FILENO); //pipe to parent deviens sortie standar
    close(outpipe[1]);
    
    execve("/bin/sh", comm, NULL);
    perror("execve");
    exit(EXIT_FAILURE);
}

void exec_last_process(int inpipe[2], char *name, char *cmd)
{
    int outfile;

    outfile = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    dup2(outfile,STDOUT_FILENO);
    close(outfile);
    close(inpipe[1]);
    dup2(inpipe[0],STDIN_FILENO);
    close(inpipe[0]);
    
    execve("/bin/sh", get_command(cmd), NULL);
    perror("execve");
    exit(EXIT_FAILURE);    
}

void launch_process(int argc, char **argv, int pipe_fd[argc - 1][2], int pid[])
{
    if(!pid[0])
        exec_first_process(argv[1], pipe_fd[pid[1] - 1]);
    else if(pid[0] && pid[1])
        exec_mid_process(pipe_fd[pid[1]], pipe_fd[pid[1] - 1], argv[argc-1-pid[1]]); 
    else
        exec_last_process(pipe_fd[pid[1]], argv[argc - 1], argv[argc-2-pid[1]]);
}

int main(int argc, char *argv[]) 
{
    int nb_cmd;
    int pipe_fd[argc - 1][2];
    int pid[2];

    nb_cmd = 0;
    pid[0] = 9;
    pid[1] = 0;
    while(nb_cmd < argc - 2)
    {
        pipe(pipe_fd[nb_cmd]);  // Crée un nouveau pipe pour chaque paire de commandes
        pid[0] = fork();
        if (pid[0])
        {
            waitpid(pid[nb_cmd], NULL, 0);
            break;
        }
        nb_cmd++;
        pid[1] = nb_cmd;
    }
    launch_process(argc, argv, pipe_fd, pid);
    return (0);
}

//inpipe = pipe_fd[pid[1]];
//outpipe = pipe_fd[pid[1] - 1];


