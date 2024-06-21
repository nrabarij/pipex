/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabarij <nrabarij@student.42antanana>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:10:28 by nrabarij          #+#    #+#             */
/*   Updated: 2024/06/18 10:22:56 by nrabarij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/// @brief Sub-function.
/// @param d The data structure.
/// @param s The pointer to the status code needed for exit.
static void	ppx(t_ppx *d, int *s)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_error(strerror(errno), "fork()", 0);
		exit(1);
	}
	if (pid == 0)
		pipex(d);
	else
	{
		ppx_close(d, 0);
		waitpid(pid, s, 0);
	}
}

/// @brief Project to reproduce the behaviour of pipe in UNIX
/// @param ac The number of arguments
/// @param av The pointer to the first element of the arguments.
/// @param en The pointer to the first element of the environement variables.
/// @return 0 on success. Non-zero value otherwise.
int	main(int ac, char **av, char **en)
{
	int		s;
	t_ppx	d;

	s = 0;
	if (ac == 5)
	{
		ppx_init(&d, ac, av, en);
		ppx(&d, &s);
		exit(WEXITSTATUS(s));
	}
	else
		ft_error("invalid arguments", PPX_USE, 1);
}
