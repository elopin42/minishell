/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bool_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:43:56 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 22:03:51 by elopin           ###   ########.fr       */
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

int	is_logic_operator(char *str)
{
	return (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2)
		|| !ft_strncmp(str, "|", 1));
}

int	is_redirection(char *str)
{
	return (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "<<", 2) || !ft_strncmp(str, ">>", 2));
}
