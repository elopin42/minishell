/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/10 18:20:37 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	save_history(void)
{
	write_history(HISTORY_FILE);
}

void	minishell_loop(t_env *ms)
{
	t_tokens	*tmp;

	setup_signals();
	while (42)
	{
		//ms->cmd_line = readline("\001\033[1;32m\002minishell>\001\033[0m\002");
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);

		rl_done = 0;
		if (isatty(fileno(stdin)))
		{
			// printf("DEBUG: Starting new readline loop\n");
			ms->cmd_line = readline("");
			// printf("DEBUG: Got line: %s\n", ms->cmd_line ? ms->cmd_line : "NULL");
		}
		else
		{
			char *line = NULL;
			line = get_next_line(fileno(stdin));
			if (line)
			{
				ms->cmd_line = ft_strtrim(line, "\n");
				free(line);
			}
			else {
				ms->cmd_line = NULL;
			}
		}
		if (!ms->cmd_line)
		{
//			printf("\033[1;31mexit\033[0m\n");
			exit_clean(ms, ms->last_exit_code);
		}
		if (ms->cmd_line && *ms->cmd_line)
			add_history(ms->cmd_line);
		if (ft_strcmp(ms->cmd_line, "\0") != 0)
		{
			ms->parse_error = 0;
			if (bout_de_scotch(ms->cmd_line))
				set_parse_error(ms, SYNTAX_ERROR);
			else
			{
				get_list_tokens(ms);
				// print_tokens(ms->tokens);
				

				tmp = dup_tokens(ms->tokens);
				
				ms->ast = get_ast(&tmp, ms);
				// fd_printf(2, "\nDEBUG\n");
				// print_error(ms->parse_error);
				// fd_printf(2, "\nDEBUG\n");
				
				// fd_printf(2, "\nMAIN AST=\n");
				// print_ast(ms->ast, 0);
				// fd_printf(2, "=MAIN AST\n\n");
			}
			if (!ms->parse_error)
			{
				consume_tree(ms->ast, ms);
			}
			else
			{
				fd_printf(2, "parse error: ");
				print_error(ms->parse_error);
				ms->last_exit_code = ms->parse_error;
			}
			if (!ms->parse_error && ms->last_exit_code)
				print_error(ms->last_exit_code);
			free(ms->cmd_line);
			ft_clear_tokens(&ms->tokens, &free);
			delete_ast(&ms->ast);
		}
	}
}

void	print_logo(void)
{
	printf("\033[1;32m");
	printf("██╗  ██╗███████╗██╗     ██╗\n");
	printf("██║  ██║██╔════╝██║     ██║\n");
	printf("███████║█████╗  ██║     ██║\n");
	printf("██╔══██║██╔══╝  ██║     ██║\n");
	printf("██║  ██║███████╗███████╗███████╗\n");
	printf("\033[0m\n");
}

char	**ft_copie_env(char **envp)
{
	int		i;
	int		j;
	char	**tab;

	i = 0;
	j = 0;
	if (!envp)
		return 0;
	while (envp[i])
		i++;
	tab = malloc((sizeof(char *)) * (i + 1));
	if (!tab)
		return (0);
	i = 0;
	while (envp[i])
	{
		j = 0;
		tab[i] = ft_calloc(ft_strlen(envp[i]) + 1, sizeof(char));
		if (!tab[i])
			return 0;
		while (envp[i][j])
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
//	print_logo();
	// read_history(HISTORY_FILE);
	ft_bzero(&ms, sizeof(t_env));
	ms.envp = ft_copie_env(envp);
	ms.argv = av;
	ft_old_and_pwd(&ms);
	minishell_loop(&ms);
}
