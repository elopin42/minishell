/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/02/25 03:25:18 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	choos_command(t_env *ms)
{
	if (ft_strcmp(ms->tokens[0], "echo") == 0 && ft_strcmp(ms->tokens[1], "-n") == 0)
		fd_printf(1, "%s", ms->tokens[2]);
	else if (ft_strcmp(ms->tokens[0], "echo") == 0)
		fd_printf(1, "%s\n",ms->tokens[1]);
}
