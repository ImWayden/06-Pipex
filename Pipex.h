/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 06:47:58 by therodri          #+#    #+#             */
/*   Updated: 2023/07/03 08:22:08 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h> // open
# include <stdio.h>
# include <stdio.h> // perror
# include <stdlib.h>
# include <stdlib.h> // malloc, free
# include <string.h>
# include <string.h>    // strerror
# include <sys/types.h> // wait, waitpid
# include <sys/wait.h>  // wait, waitpid
# include <unistd.h>    // open, close, read, write, access, dup, dup2, execve,

void	launch_process(int argc, char **argv, int pipe_fd[2][2], int pid[2]);
char	**get_command(char *cmd);
#endif
