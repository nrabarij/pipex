/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabarij <nrabarij@student.42antanana>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:06:29 by nrabarij          #+#    #+#             */
/*   Updated: 2024/06/18 10:32:38 by nrabarij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include "../libft/libft.h"

# ifndef PPX_USE
#  define PPX_USE "./pipex \"input\" cmd1 cmd2 \"output\""
# endif

typedef struct s_ppx
{
	int				id;
	int				ac;
	int				fd[2];
	char			**av;
	char			**en;
	int				ipf;
	int				ioe;
	int				opf;
	int				ooe;
}					t_ppx;

char	*ppx_cmd(char *cmd, char **en);
void	ppx_proc_exit(t_ppx *d, int code);
void	ppx_close(t_ppx *d, int code);
void	ppx_init(t_ppx *d, int ac, char **av, char **en);
void	pipex(t_ppx *d);
char	**ppx_split(const char *s, int c);

#endif
