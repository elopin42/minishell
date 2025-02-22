/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/22 23:35:44 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    minishell_loop(t_env *ms)
{
    while (1)
    {
        ms->cmd_line = readline("minishell> ");
        fd_printf(1, "%s\n", ms->cmd_line);
        ms->tokens = get_tokens(ms);
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
