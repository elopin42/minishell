/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:40:07 by elopin            #+#    #+#             */
/*   Updated: 2025/06/13 00:18:41 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_command(t_env *ms)
{
	parse_command(ms);
	execute_or_error(ms);
	cleanup_command(ms);
}

void	execute_or_error(t_env *ms)
{
	if (!ms->parse_error)
		consume_tree(ms->ast, ms);
	else
		handle_parse_error(ms);
	if (!ms->parse_error && ms->last_exit_code)
		print_error(ms->last_exit_code);
}
