/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:15:41 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/16 17:30:27 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("SIGINT error\n");
	}
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("SIGQUIT error\n");
	}

	sa.sa_handler = SIG_IGN;
    sigaction(SIGTSTP, &sa, NULL);
}

void	handler(int	sig)
{
	(void)	sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
