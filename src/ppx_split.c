/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabarij <nrabarij@student.42antanana>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:49:47 by nrabarij          #+#    #+#             */
/*   Updated: 2024/06/17 21:35:05 by nrabarij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static void	st_init(int i[4], int code)
{
	if (code == 0)
	{
		i[0] = -1;
		i[1] = 0;
		i[2] = 0;
		i[3] = 0;
	}
	else if (code == 1)
	{
		i[0] = 0;
		i[1] = -1;
		i[2] = 0;
		i[3] = 0;
	}
}

/// @brief Find the corresponding closing bracket y moving the index.
/// @param s The string to iterate through.
/// @param obr The open bracket.
/// @param i The pointer to the value of index
static void	st_move_id(const char *s, int obr, int *i)
{
	int	cbr;

	cbr = ft_cbr(s[obr]);
	(*i)++;
	while (s[(*i)] && s[*i] != cbr)
		(*i)++;
	if (s[*i] == cbr)
		(*i)++;
}

/// @brief Count the number of words while taking brackets into consideration.
/// @param s The string to check.
/// @param c The separator (usually, a space).
/// @return The number of words in the string (between brackets == 1 word).
static size_t	st_wc(const char *s, int c)
{
	int	i[4];

	if (!s)
		return (0);
	st_init(i, 1);
	while (s[i[0]])
	{
		if (ft_isobr(s[i[0]]) && i[1] < 0)
		{
			i[1] = i[0];
			st_move_id(s, i[1], &(i[0]));
		}
		if (s[i[0]] != c && i[2] == 0)
		{
			i[2] = 1;
			(i[3])++;
		}
		else if (s[i[0]] == c)
			i[2] = 0;
		if (s[i[0]] == '\0')
			break ;
		(i[0])++;
	}
	return (i[3]);
}

/// @brief Copy the word to the corresponding table index.
/// @param strs The table.
/// @param i The indexes needed for ft_split().
/// @param s The string to split.
/// @return The table strs.
static char	**st_wrdcpy(char **strs, int i[4], const char *s)
{
	char	*tmp;

	if (ft_isobr(s[i[0]]))
		st_move_id(s, s[i[0]], &(i[1]));
	strs[i[2]] = (char *)malloc(sizeof(char) * (i[1] - i[0] + 1));
	if (!(strs[i[2]]))
	{
		while ((i[2]) >= 0)
			free(strs[(i[2])--]);
		return (free(strs), NULL);
	}
	i[3] = 0;
	while (i[0] < i[1])
		strs[i[2]][(i[3])++] = s[(i[0])++];
	strs[i[2]][i[3]] = '\0';
	if (strs[i[2]][0] == 34 || strs[i[2]][0] == 39)
	{
		tmp = ft_strtrim(strs[i[2]], "\'\"");
		free(strs[i[2]]);
		strs[i[2]] = tmp;
	}
	i[0] = -1;
	i[2]++;
	return (strs);
}

/// @brief Split the command argument into a 2D array needed by execve.
/// @param s The string to split.
/// @param c The separator (usually a space).
/// @return The splitted string on success. NULL otherwise.
char	**ppx_split(const char *s, int c)
{
	int		i[4];
	char	**arg;
	int		ss[2];

	if (!s)
		return (NULL);
	arg = (char **)malloc((st_wc(s, c) + 1) * sizeof(arg));
	if (!arg)
		return (NULL);
	st_init(i, 0);
	ss[0] = (int)ft_strlen(s);
	ss[1] = (int)st_wc(s, c);
	while (i[1] <= ss[0] && i[2] < ss[1])
	{
		if (s[(i[1])] != c && (i[0]) < 0)
			(i[0]) = (i[1]);
		else if ((s[(i[1])] == c || (i[1]) == ss[0] || ft_isobr(s[i[1]]))
			&& (i[0]) >= 0)
			arg = st_wrdcpy(arg, i, s);
		if (arg == NULL)
			return (NULL);
		(i[1])++;
	}
	arg[(i[2])] = NULL;
	return (arg);
}
