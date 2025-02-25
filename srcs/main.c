/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/25 04:41:11 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//j'ai mit un split en attendant ton parsing 
//segfault avec \n

void    minishell_loop(t_env *ms)
{
	setup_signals();
    while (42)
    {
        ms->cmd_line = readline("minishell> ");
		if (!ms->cmd_line)
		{
			fd_printf(1, "exit\n");
			exit (0);
		}
		get_tokens(ms);
		choos_command(ms);
	//	print_tab(ms->tokens);
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
    minishell_loop(&ms);
}
