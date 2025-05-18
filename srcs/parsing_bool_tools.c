/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bool_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:43:56 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/04 18:56:10 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_operand1(char *str)
{
	if (!ft_strncmp(str, "|", 1) || !ft_strncmp(str, "<", 1) || !ft_strncmp(str,
			">", 1) || !ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
		return (1);
	return (0);
}

int	is_operand2(char *str)
{
	if (!ft_strncmp(str, "<<", 2) || !ft_strncmp(str, ">>", 2)
		|| !ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2))
		return (1);
	return (0);
}

int	is_operand(char *str)
{
	if (!ft_strncmp(str, "|", 1) || !ft_strncmp(str, "<", 1) || !ft_strncmp(str,
			">", 1) || !ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1)
		|| !ft_strncmp(str, "<<", 2) || !ft_strncmp(str, ">>", 2)
		|| !ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2))
		return (1);
	return (0);
}

bool	is_dquotes(char *tmp)
{
	return (tmp[0] == '\"' && tmp[ft_strlen(tmp) - 1] == '\"');
}

bool	is_squotes(char *tmp)
{
	return (tmp[0] == '\'' && tmp[ft_strlen(tmp) - 1] == '\'');
}