/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:20:25 by tbeauman          #+#    #+#             */
/*   Updated: 2024/11/10 14:01:03 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	len_word(const char *str, int i, char c)
{
	int	ret;

	ret = 0;
	while (str[i] != c && str[i])
	{
		i++;
		ret++;
	}
	return (ret);
}

static int	count_words(const char *str, char c)
{
	int	r;
	int	i;

	r = 0;
	i = 0;
	if (str[i] == c)
		while (str[i] && str[i] == c)
			i++;
	while (str[i])
	{
		if (str[i + 1] == '\0' && str[i] != c)
			r++;
		if (str[i] == c)
		{
			r++;
			while (str[i + 1] == c)
			{
				i++;
			}
		}
		i++;
	}
	return (r);
}

static int	clear_res(int l, int nw, char **res)
{
	while (l < nw)
		free(res[l++]);
	free(res);
	return (1);
}

static char	**process(const char *str, int i[2], char c, char **res)
{
	int	l;

	l = 0;
	res[i[1]] = (char *)malloc(sizeof(char) * (len_word(str, i[0], c) + 1));
	if (!res[i[1]] && clear_res(l, i[1], res))
		return (NULL);
	while (str[i[0]] != c && str[i[0]])
		res[i[1]][l++] = str[i[0]++];
	res[i[1]++][l] = '\0';
	return (res);
}

char	**ft_split(const char *str, char c)
{
	int		i[2];
	char	**res;

	res = (char **)malloc(sizeof(char *) * (count_words(str, c) + 1));
	if (!res)
		return (NULL);
	i[0] = 0;
	i[1] = 0;
	while (str[i[0]])
	{
		while (str[i[0]] == c)
			i[0]++;
		if (str[i[0]] && !process(str, i, c, res))
			return (NULL);
	}
	res[i[1]] = NULL;
	return (res);
}
