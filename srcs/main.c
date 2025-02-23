/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/23 04:21:18 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handler(int	sig)
{
	(void)	sig;
	exit(fd_printf(2, "bye..."));
	// bug si \n a la fin du bye meme si fd a 1
}

void    minishell_loop(t_env *ms)
{
    while (42)
    {
        ms->cmd_line = readline("minishell> ");
//        fd_printf(1, "%s\n", ms->cmd_line);
        get_tokens(ms);
		fd_printf(1, "good\n");
		print_tab(ms->tokens);
		// tout marche mais le ms->tokens ne passe pas dans print_tab
		signal(SIGINT, handler);
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
