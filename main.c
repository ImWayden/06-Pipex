/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 04:45:55 by wayden            #+#    #+#             */
/*   Updated: 2023/07/03 08:21:57 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pipex.h"

char	**get_command(char *cmd)
{
	static char	*full_cmd[] = {"sh", "-c", NULL, NULL};

	full_cmd[2] = cmd;
	return (full_cmd);
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
