/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_misc_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:50:26 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/04 15:13:14 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	compute_end_operand_index(char *str, int i, int *start)
{
	(void)start;
	if (is_operand2(str))
		return (i + 2);
	else
		return (i + 1);
}

int	is_in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	count_chars_to_remove(char const *s1, char const *set)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (s1[i])
	{
		if (is_in_set(s1[i], set))
			ret++;
		i++;
	}
	return (ret);
}

char	*ft_strtrimv2(char const *s1, char const *set)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = (char *)malloc((ft_strlen(s1) - count_chars_to_remove(s1, set)) + 1);
	if (!ret)
		return (0);
	while (s1[j])
	{
		if (!is_in_set(s1[j], set))
		{
			ret[i] = s1[j];
			i++;
		}
		j++;
	}
	ret[i] = 0;
	return (ret);
}
