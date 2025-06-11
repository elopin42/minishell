/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bool_tools2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:03:11 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 22:04:00 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_one_dquotes(char *tmp)
{
	int	i;

	if (tmp[0] != '"')
		return (0);
	i = 1;
	while (tmp[i + 1])
	{
		if (tmp[i] == '"')
			return (0);
		i++;
	}
	if (tmp[i] != '"')
		return (0);
	return (1);
}

bool	is_dquotes(char *tmp)
{
	return (tmp[0] == '\"' && tmp[ft_strlen(tmp) - 1] == '\"');
}

bool	is_one_squotes(char *tmp)
{
	int	i;

	if (tmp[0] != '\'')
		return (0);
	i = 1;
	while (tmp[i + 1])
	{
		if (tmp[i] == '\'')
			return (0);
		i++;
	}
	if (tmp[i] != '\'')
		return (0);
	return (1);
}

bool	is_squotes(char *tmp)
{
	return (tmp[0] == '\'' && tmp[ft_strlen(tmp) - 1] == '\'');
}
