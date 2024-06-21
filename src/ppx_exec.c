/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabarij <nrabarij@student.42antanana>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:10:28 by nrabarij          #+#    #+#             */
/*   Updated: 2024/06/18 10:18:44 by nrabarij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/// @brief Execute the command passed into parameter.
/// @param cmd The command to execute.
/// @param arg The argument of the command, if any.
/// @param d The data structure containing the environement variables.
/// @param code Check if cmd was allocated separately form arg.
static void	ppx_exec(char *cmd, char **arg, t_ppx *d, int code)
{
	int	i;

	i = 1;
	if (!cmd)
	{
		i = 0;
		cmd = ".";
	}
	if (execve(cmd, arg, d->en) == -1)
	{
		if (i == 0)
		{
			ft_error("command not found", arg[0], 0);
			ft_close(3, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
			return (ft_free_tab(arg), ppx_close(d, 1), exit(127));
		}
		else if (i == 1)
			ft_error(strerror(errno), NULL, 0);
		if (code && i)
			free(cmd);
		ft_close(3, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
		return (ft_free_tab(arg), ppx_close(d, 1), exit(1));
	}
	exit(0);
}

/// @brief Function to read from a file or from the pipe and write to the pipe.
/// @param d The data structure.
static void	ppx_io(t_ppx *d, int code)
{
	char	*cmd;
	char	**arg;

	if (code == 0)
	{
		if (d->ipf < 0)
			ppx_proc_exit(d, 1);
		dup2(d->ipf, STDIN_FILENO);
		dup2(d->fd[1], STDOUT_FILENO);
	}
	else if (code == 1)
	{
		if (d->opf < 0)
			ppx_proc_exit(d, 0);
		dup2(d->ipf, STDIN_FILENO);
		dup2(d->opf, STDOUT_FILENO);
	}
	ft_close(4, d->fd[0], d->fd[1], d->ipf, d->opf);
	arg = ppx_split(d->av[d->id], 32);
	if (!arg)
		ft_error(strerror(errno), NULL, 1);
	if (arg && arg[0] && arg[0][0] && access(arg[0], F_OK | X_OK) == 0)
		ppx_exec(arg[0], arg, d, 0);
	cmd = ppx_cmd(arg[0], d->en);
	return (ppx_exec(cmd, arg, d, 1), exit(0));
}

/// @brief Check if a command exists. Needed to set the waitpid() option.
/// @param cmd The command to find.
/// @param d The data structure with environement variables.
/// @return 1 if the command is found, zero otherwise.
static int	ppx_chk_cmd(char *cmd, t_ppx *d)
{
	char	*tmp;
	char	**arg;

	if (!cmd || !(*cmd))
		return (0);
	arg = ppx_split(cmd, 32);
	if (!arg)
		ft_error(strerror(errno), NULL, 0);
	if (access(arg[0], F_OK) == 0)
		return (ft_free_tab(arg), 1);
	tmp = ppx_cmd(arg[0], d->en);
	ft_free_tab(arg);
	if (!tmp)
		return (0);
	else
		return (free(tmp), 1);
}

/// @brief Function to handle multiple pipes. The closing and waiting process.
/// @param d The data structure.
/// @param pid The pointer to the pid needed for waitpid() function.
/// @param proc Check if close is needed and next process or if wait is needed.
static void	ppx_pipe(t_ppx *d, pid_t *pid, int proc)
{
	if (proc == 0)
	{
		ft_close(2, d->ipf, d->fd[1]);
		d->ipf = d->fd[0];
		(d->id)++;
		if (d->id == d->ac - 1)
			ppx_io(d, 1);
	}
	else if (proc == 1)
	{
		if (d->av[d->id + 1] && ppx_chk_cmd(d->av[d->id + 1], d))
			waitpid((*pid), NULL, WNOHANG);
		else
			waitpid((*pid), NULL, 0);
	}
}

/// @brief Reproduce the behaviour of pipe in UNIX.
/// @param d The data structure.
void	pipex(t_ppx *d)
{
	pid_t	pid;

	while (d->id < d->ac)
	{
		if (pipe(d->fd) == -1)
		{
			return (ft_error(strerror(errno), "pipe()", 0), ppx_close(d, 0),
				exit(1));
		}
		pid = fork();
		if (pid < 0)
			return (ft_error(strerror(errno), "fork()", 0), ppx_close(d, 1),
				exit(1));
		if (pid == 0)
			ppx_io(d, 0);
		else
		{
			if (!ft_strncmp(d->av[d->id], "sleep", 5))
				wait(NULL);
			else
				ppx_pipe(d, &pid, 1);
			ppx_pipe(d, NULL, 0);
		}
	}
}
