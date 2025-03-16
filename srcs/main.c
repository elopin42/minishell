/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/16 18:45:03 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//j'ai mit un split en attendant ton parsing 
//segfault avec \n

void    minishell_loop(t_env *ms)
{
    pid_t   pid;
    int     i;

    i = 0;
	setup_signals();
    while (42)
    {
        ms->cmd_line = readline("minishell> ");
		if (!ms->cmd_line)
		{
			fd_printf(1, "exit\n");
			exit_clean(ms, 0);
		}
		if (ft_strcmp(ms->cmd_line, "\0" ) != 0)
		{
		//	printf("ft_strcmp == %d\n", ft_strcmp(ms->cmd_line, "\0"));
			if (ms->cmd_line)
            	add_history(ms->cmd_line);
			get_tokens(ms);
            print_tab(ms->tokens);
            ms->ast = get_ast(ms->tokens, &i);
            print_ast(ms->ast, 0);

   	    	if (ft_strcmp(ms->tokens[0], "exit") == 0)
			{
				fd_printf(1, "exit\n");
				exit_clean(ms, 0);
			}
            pid = fork();
        	ms->pididi = pid;
   	    	if (pid == -1)
        	{
           		perror("fork");
            	exit_clean(ms, EXIT_FAILURE);
        	}
        	if (pid == 0)
            	execute(ms);
        	else
        	{
            	wait(NULL);
            	// fd_printf(2, "%d %s\n", __LINE__, __FILE__);
        	}
        // choose_command(ms);
		}
	//	print_tab(ms->tokens);
    }
}

void print_logo(void)
{
    printf("\033[1;32m");
    printf("███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n");
    printf("████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n");
    printf("██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n");
    printf("██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n");
    printf("██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n");
    printf("\033[0m\n");
}

int     main(const int ac, const char **av, char **envp)
{
    t_env   ms;

    (void)ac;
    (void)av;
	print_logo();
    ft_bzero(&ms, sizeof(t_env));
    ms.envp = envp;
    minishell_loop(&ms);
}
