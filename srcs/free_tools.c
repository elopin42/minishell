/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:18:45 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/16 17:25:51 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup(t_env *ms)
{
    free_tab(ms->tokens);
}

void	exit_clean(t_env *ms, int exit_code)
{
	cleanup(ms);
	if (ms->pididi != 0)
		kill(ms->pididi, SIGKILL);
	exit(exit_code);
}

void 	free_tab(char **tab)
{
	int	i;

	i = 0;
    if (!tab)
    {
        return ;
    }
    while(tab[i])
    {
	    free(tab[i]);
        i++;
    }
    free(tab);
}
