/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/30 17:54:27 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// j'ai mit un split en attendant ton parsing
// segfault avec \n

void	save_history(void)
{
	write_history(HISTORY_FILE);
}

void	minishell_loop(t_env *ms)
{
	setup_signals();
	while (42)
	{
		ms->cmd_line = readline("minishell> ");
		if (!ms->cmd_line)
		{
			fd_printf(1, "exit de fdp\n");
			exit_clean(ms, 0);
		}
		if (ms->cmd_line && *ms->cmd_line)
			add_history(ms->cmd_line);
		if (ft_strcmp(ms->cmd_line, "\0") != 0)
		{
			ft_clear_tokens(&ms->tokens, &free);
			get_list_tokens(ms);
			// print_tokens(ms->tokens);
			ms->ast = get_ast(ms->tokens);
			fd_printf(1, "===MAIN===\n");
			print_ast(ms->ast, 0);
			fd_printf(1, "===MAIN===\n\n");
			consume_tree(ms->ast, ms);
			delete_ast(&ms->ast);
		}
	}
}

void	print_logo(void)
{
	printf("\033[1;32m");
	printf("███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n");
	printf("████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n");
	printf("██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n");
	printf("██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n");
	printf("██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n");
	printf("\033[0m\n");
}

int	main(const int ac, const char **av, char **envp)
{
	t_env	ms;

	(void)ac;
	(void)av;
	print_logo();
	read_history(HISTORY_FILE);
	ft_bzero(&ms, sizeof(t_env));
	ms.envp = envp;
	minishell_loop(&ms);
}
