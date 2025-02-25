/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/02/25 04:50:27 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	choos_command(t_env *ms)
{
	if (ft_strcmp(ms->tokens[0], "echo") == 0 && ft_strcmp(ms->tokens[1], "-n") == 0)
		fd_printf(1, "%s", ms->tokens[2]);
	else if (ft_strcmp(ms->tokens[0], "echo") == 0)
		fd_printf(1, "%s\n",ms->tokens[1]);
	else if (ft_strcmp(ms->tokens[0], "exit") == 0)
	{
		free_tab(ms);
		fd_printf(1, "exit\n");
		exit (0);
	}
	free_tab(ms);
}

void 	free_tab(t_env *ms)
{
	int	i;

	i = -1;
	while(ms->tokens[++i])
		free(ms->tokens[i]);
	free(ms->tokens);
}
