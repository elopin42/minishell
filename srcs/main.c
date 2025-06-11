/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:11:15 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 23:08:51 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ms->cmd_line = readline("\001\033[1;32m\002minishell>\001\033[0m\002");
/*if (isatty(fileno(stdin)))
		{
			ms->cmd_line = readline("sisi>");
		}
		else
		{
			line = NULL;
			line = get_next_line(fileno(stdin));
			if (line)
			{
				ms->cmd_line = ft_strtrim(line, "\n");
				free(line);
			}
			else
				ms->cmd_line = NULL;
		} */

void	handle_sigint_readline(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*void	minishell_loop(t_env *ms)
{
	t_tokens	*tmp;
	char		*line;

	setup_signals();
	while (42)
	{
		signal(SIGINT, handle_sigint_readline);
		signal(SIGQUIT, SIG_IGN);
		rl_done = 0;
		ms->cmd_line = readline("\001\033[1;32m\002minishell>\001\033[0m\002");
		if (!ms->cmd_line)
			exit_clean(ms, ms->last_exit_code);
		if (ms->cmd_line && *ms->cmd_line)
			add_history(ms->cmd_line);
		if (ft_strcmp(ms->cmd_line, "\0") != 0)
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			ms->parse_error = 0;
			if (check_edges(ms->cmd_line))
				set_parse_error(ms, SYNTAX_ERROR);
			else
			{
				get_list_tokens(ms);
				tmp = dup_tokens(ms->tokens);
				ms->ast = get_ast(&tmp, ms);
			}
			if (!ms->parse_error)
				consume_tree(ms->ast, ms);
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
		else
			free(ms->cmd_line);
	}
}*/
void	minishell_loop(t_env *ms)
{
	setup_signals();
	while (42)
	{
		setup_readline_signals();
		if (get_user_input(ms))
			process_command(ms);
		else
			free(ms->cmd_line);
	}
}

char	**ft_copie_env(char **envp)
{
	char	**tab;

	int (i) = 0;
	int (j) = 0;
	if (!envp)
		return (0);
	while (envp[i])
		i++;
	tab = malloc((sizeof(char *)) * (i + 1));
	if (!tab)
		return (0);
	i = -1;
	while (envp[++i])
	{
		j = 0;
		tab[i] = ft_calloc(ft_strlen(envp[i]) + 1, sizeof(char));
		if (!tab[i])
			return (0);
		while (envp[i][j])
		{
			tab[i][j] = envp[i][j];
			j++;
		}
	}
	return (tab[i] = NULL, tab);
}

int	main(int ac, char **av, char **envp)
{
	t_env	ms;

	if (ac != 1)
		return (printf("error\n"), 126);
	print_logo();
	ft_bzero(&ms, sizeof(t_env));
	ms.envp = ft_copie_env(envp);
	ms.argv = av;
	ft_old_and_pwd(&ms);
	minishell_loop(&ms);
}
