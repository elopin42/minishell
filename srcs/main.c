/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/04/11 21:56:37 by elopin           ###   ########.fr       */
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
		ms->err_flag = 0;
		ms->cmd_line = readline("minishell> ");
		if (!ms->cmd_line)
		{
			fd_printf(1, "exit de fdp\n");
			fd_printf(1, "%d\n", sizeof(t_tokens));
			exit_clean(ms, 0);
		}
		if (ms->cmd_line && *ms->cmd_line)
			add_history(ms->cmd_line);
		if (ft_strcmp(ms->cmd_line, "\0") != 0)
		{
			get_list_tokens(ms);
			fd_printf(1, "===TOKENS===\n");
			print_tokens(ms->tokens);
			fd_printf(1, "===TOKENS===\n");
			
			t_tokens *tmp;

			tmp = dup_tokens(ms->tokens);
			ms->ast = get_ast(&tmp, ms);

			fd_printf(1, "===AVANT===\n");
			print_ast(ms->ast, 0);
			fd_printf(1, "===AVANT===\n\n");

			// expand_tokens(ms->ast, ms);
			// fd_printf(1, "===APRES===\n");
			// print_ast(ms->ast, 0);
			// fd_printf(1, "===APRES===\n\n");
			if (!ms->err_flag)
				consume_tree(ms->ast, ms);
			else
				fd_printf(2, "error\n");
			free(ms->cmd_line);
			ft_clear_tokens(&ms->tokens, &free);
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

char **ft_copie_env(char **envp)
{
	int	i;
	int	j;
	char **tab;

	i = 0;
	j = 0;
	while(envp[i])
		i++;
	tab = malloc((sizeof(char *)) * (i + 1));
	if (!tab)
		return (0);
	i = 0;
	while(envp[i])
	{
		j = 0;
		tab[i] = ft_calloc(ft_strlen(envp[i]) + 1, sizeof(char));
		while(envp[i][j])
		{
			tab[i][j] = envp[i][j];
			j++;
		}
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

int	main(int ac, char **av, char **envp)
{
	t_env	ms;

	(void)ac;
	print_logo();
	read_history(HISTORY_FILE);
	ft_bzero(&ms, sizeof(t_env));
	ms.envp = ft_copie_env(envp);
	ms.argv = av;
	ft_old_and_pwd(&ms);
	minishell_loop(&ms);
}
