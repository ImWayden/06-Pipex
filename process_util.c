/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 08:20:35 by therodri          #+#    #+#             */
/*   Updated: 2023/07/03 08:21:26 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pipex.h"

static void	exec_first_process(char *infile, int outpipe[2], char *cmd)
{
	int	file;

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

static void	exec_mid_process(int inpipe[2], int outpipe[2], char *cmd)
{
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

static void	exec_last_process(int inpipe[2], char *name, char *cmd)
{
	int	outfile;

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
	if (!pid[0] && pid[1])
		exec_first_process(argv[1], pipe_fd[1], argv[argc - pid[1] - 2]);
	else if (pid[0] && pid[1])
		exec_mid_process(pipe_fd[0], pipe_fd[1], argv[argc - pid[1] - 2]);
	else if (pid[0] && !pid[1])
		exec_last_process(pipe_fd[0], argv[argc - 1], argv[argc - pid[1] - 2]);
}