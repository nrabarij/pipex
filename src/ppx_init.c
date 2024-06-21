/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabarij <nrabarij@student.42antanana>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:10:28 by nrabarij          #+#    #+#             */
/*   Updated: 2024/06/18 10:32:04 by nrabarij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/// @brief Initialize the data structure.
/// @param d The pointer to the data structure.
/// @param ac The number of argument.
/// @param av The pointer to the first argument.
/// @param en The pointer to the first environement variable,
void	ppx_init(t_ppx *d, int ac, char **av, char **en)
{
	if (!d)
		ft_error("data structure unavailable", NULL, 1);
	d->ac = ac - 1;
	d->av = av;
	d->en = en;
	d->id = 2;
	d->ioe = 0;
	d->ooe = 0;
	d->ipf = open(d->av[1], O_RDONLY);
	if (d->ipf < 0)
		d->ioe = errno;
	d->opf = open(d->av[d->ac], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (d->opf < 0)
		d->ooe = errno;
}

/// @brief Close any opened fd, including pipe fds.
/// @param d The data structure.
/// @param code Check if pipe should also be closed.
void	ppx_close(t_ppx *d, int code)
{
	if (d->ioe == 0)
		close(d->ipf);
	if (d->ooe == 0)
		close(d->opf);
	if (code == 1)
		ft_close(2, d->fd[0], d->fd[1]);
}

/// @brief Exit the ppx_proc() function in case of fd open() error.
/// @param d The data structure with pipe fds.
/// @param code Check which pipe end to close.
void	ppx_proc_exit(t_ppx *d, int code)
{
	if (code == 0)
		ft_error(strerror(d->ooe), d->av[d->ac], 0);
	else if (code == 1)
		ft_error(strerror(d->ioe), d->av[1], 0);
	ppx_close(d, 1);
	exit(1);
}

/// @brief Find the "PATH" variables from a given environement.
/// @param en The pointer to the first element of the environement.
/// @return A non-NULL pointer on success.
static char	**ppx_path(char **en)
{
	int		i;
	char	**path;

	if (!en)
	{
		ft_error("Environement", NULL, 0);
		return (NULL);
	}
	i = 0;
	path = NULL;
	while (en[i] && !ft_strnstr(en[i], "PATH=", 5))
		i++;
	if (en[i])
		path = ft_split(&(en[i][5]), 58);
	else if (!en[i])
	{
		ft_error("Environement", NULL, 0);
		return (NULL);
	}
	if (!path)
		ft_error(strerror(errno), NULL, 0);
	return (path);
}

/// @brief Find the command from a given environement path variable.
/// @param cmd The command to find.
/// @param en The pointer to the first element of the environement.
/// @return A non-NULL pointer on success.
char	*ppx_cmd(char *cmd, char **en)
{
	int		i;
	char	*c;
	char	*tmp;
	char	**path;

	i = -1;
	if (!cmd || !en)
		return (NULL);
	path = ppx_path(en);
	if (!path)
		return (NULL);
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (ft_free_tab(path), perror(FT_ERR), NULL);
		c = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!c)
			return (ft_free_tab(path), perror(FT_ERR), NULL);
		if (access(c, F_OK | X_OK) == 0)
			return (ft_free_tab(path), c);
		free(c);
	}
	return (ft_free_tab(path), NULL);
}
