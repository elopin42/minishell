/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/23 23:49:13 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>

void	handler(int	sig)
{
	(void)	sig;
	exit(127);
	// bug si \n a la fin du bye meme si fd a 1
}

void    minishell_loop(t_env *ms)
{
    while (42)
    {
        ms->cmd_line = readline("minishell> ");
//        fd_printf(1, "%s\n", ms->cmd_line);
        get_tokens(ms);
//		fd_printf(1, "%s:%d good\n", __FILE__, __LINE__);
		print_tab(ms->tokens);
		// tout marche mais le ms->tokens ne passe pas dans print_tab
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
