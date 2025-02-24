/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/24 18:25:33 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>



void    minishell_loop(t_env *ms)
{
    while (42)
    {
        ms->cmd_line = readline("minishell> ");
        get_tokens(ms);
		print_tab(ms->tokens);
		signal(SIGINT, handler);
//		signal(SIGSTOP, handler);
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
