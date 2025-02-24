/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/24 23:19:55 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>



void    minishell_loop(t_env *ms)
{
    while (42)
    {
        ms->cmd_line = readline("minishell> ");
		if (!ms->cmd_line)
		{
			fd_printf(1, "EXIT ...");
			exit (0);
		}
        get_tokens(ms);
		print_tab(ms->tokens);
    }
}

int     main(const int ac, const char **av, const char **envp)
{
    t_env   ms;

    (void)ac;
    (void)av;
    (void)envp;
    ft_bzero(&ms, sizeof(t_env));
    ms.envp = envp;
	setup_signals();
    minishell_loop(&ms);
}
