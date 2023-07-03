/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 04:45:55 by wayden            #+#    #+#             */
/*   Updated: 2023/07/03 08:18:22 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pipex.h"

char	**get_command(char *cmd)
{
	static char	*full_cmd[] = {"sh", "-c", NULL, NULL};

	full_cmd[2] = cmd;
	return (full_cmd);
}

void	exec_first_process(char *infile, int outpipe[2], char *cmd)
{
	int	file;

	printf("getting in first for %s\n", cmd);
	file = open(infile, O_RDONLY | O_TRUNC, 0777);
	dup2(file, STDIN_FILENO);
	close(file);
	close(outpipe[0]);
	dup2(outpipe[1], STDOUT_FILENO);
	close(outpipe[1]);
	execve("/bin/sh", get_command(cmd), NULL);
	perror("Erreur lors de l'exécution de cat");
	exit(EXIT_FAILURE);
}

void	exec_mid_process(int inpipe[2], int outpipe[2], char *cmd)
{
	char	**comm;

	printf("getting in mid for %s\n", cmd);
	close(inpipe[1]);
	close(outpipe[0]);
	dup2(inpipe[0], STDIN_FILENO);
	close(inpipe[0]);
	dup2(outpipe[1], STDOUT_FILENO);
	close(outpipe[1]);
	execve("/bin/sh", get_command(cmd), NULL);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	exec_last_process(int inpipe[2], char *name, char *cmd)
{
	int	outfile;

	printf("getting in last for %s\n", cmd);
	outfile = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close(inpipe[1]);
	dup2(inpipe[0], STDIN_FILENO);
	close(inpipe[0]);
	execve("/bin/sh", get_command(cmd), NULL);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	launch_process(int argc, char **argv, int pipe_fd[2][2], int pid[2])
{
	printf("before exec pid[0] = %d  pid[1] = %d\n", pid[0], pid[1]);
	if (!pid[0] && pid[1])
		exec_first_process(argv[1], pipe_fd[1], argv[argc - pid[1] - 2]);
	else if (pid[0] && pid[1])
		exec_mid_process(pipe_fd[0], pipe_fd[1], argv[argc - pid[1] - 2]);
	else if (pid[0] && !pid[1])
		exec_last_process(pipe_fd[0], argv[argc - 1], argv[argc - pid[1] - 2]);
}

int	main(int argc, char *argv[])
{
	int	nb_cmd;
	int	pipe_fd[2][2];
	int	pid[2];

	nb_cmd = 0;
	pid[0] = 9;
	pid[1] = 0;
	while (nb_cmd < argc - 4)
	{
		pipe(pipe_fd[0]);
		pid[0] = fork();
		if (pid[0])
		{
			waitpid(pid[0], NULL, 0);
			break ;
		}
		else
			memcpy(pipe_fd[1], pipe_fd[0], sizeof(pipe_fd[0]));
		nb_cmd++;
		pid[1] = nb_cmd;
	}
	launch_process(argc, argv, pipe_fd, pid);
	return (0);
}

//inpipe = pipe_fd[pid[1]];
//outpipe = pipe_fd[pid[1] - 1];
