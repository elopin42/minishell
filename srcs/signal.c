/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:24:18 by elopin            #+#    #+#             */
/*   Updated: 2025/02/24 23:19:38 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*void 	free_exit_touch(t_env *ms)
{
	static char ***tab = NULL;
	int	i;

	i = -1;
	if (ms)
		tab = &ms->tokens;
	else 
	{
		while((*tab)[++i])
			free((*tab)[i]);
		free((*tab));
	}
}*/

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
}



void	handler(int	sig)
{
	(void)	sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
