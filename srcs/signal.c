/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:24:18 by elopin            #+#    #+#             */
/*   Updated: 2025/02/24 18:36:05 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void 	free_exit_touch(t_env *ms)
{
	static char ***tab = NULL;
	int	i;

	i = -1;
	if (ms)
		tab = &ms->tokens;
	else 
	{
		while(tab[++i])
			free(tab[i]);
		free(tab);
	}
}

void	handler(int	sig)
{
	(void)	sig;
	exit(127);
}
