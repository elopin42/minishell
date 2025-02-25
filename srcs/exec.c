/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/02/25 03:08:08 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	choos_command(t_env *ms)
{
	if (ft_strcmp(ms->tokens[0], "echo") == 0)
		fd_printf(1, ms->tokens[1]);
	else
		return ;
	write(1, "\n", 1);
}
