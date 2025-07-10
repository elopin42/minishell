/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:39:04 by elopin            #+#    #+#             */
/*   Updated: 2025/06/13 00:16:55 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_readline_signals(void)
{
	signal(SIGINT, handle_sigint_readline);
	signal(SIGQUIT, SIG_IGN);
	rl_done = 0;
}

int	get_user_input(t_env *ms)
{
	ms->cmd_line = readline("\001\033[1;32m\002minishell>\001\033[0m\002");
	if (!ms->cmd_line)
	{
		printf("\033[1;31mexit\033[0m\n");
		exit_clean(ms, ms->last_exit_code);
	}
	if (ms->cmd_line && *ms->cmd_line)
		add_history(ms->cmd_line);
	return (ft_strcmp(ms->cmd_line, "\0") != 0);
}

void	parse_command(t_env *ms)
{
	t_tokens	*tmp;

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
}

void	handle_parse_error(t_env *ms)
{
	fd_printf(2, "parse error: ");
	print_error(ms->parse_error);
	ms->last_exit_code = ms->parse_error;
}

void	cleanup_command(t_env *ms)
{
	free(ms->cmd_line);
	ft_clear_tokens(&ms->tokens, &free);
	delete_ast(&ms->ast);
}
