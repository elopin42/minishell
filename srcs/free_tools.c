/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:18:45 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:52:23 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup(t_env *ms)
{
	free(ms->cmd_line);
	ft_clear_tokens(&ms->tokens, &free);
	delete_ast(&ms->ast);
	free_tab(ms->envp);
	close_all_fds();
}

void	close_all_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	exit_clean(t_env *ms, int exit_code)
{
	cleanup(ms);
	if (ms->pididi != 0)
		kill(ms->pididi, SIGKILL);
	save_history();
	ms->last_exit_code = exit_code;
	exit(exit_code);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
	{
		return ;
	}
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_token(t_tokens **tok)
{
	if (!*tok)
		return ;
	if ((*tok)->type == MULTI && (*tok)->parts)
		free_token_parts(&(*tok)->parts);
	else if ((*tok)->token)
		free((*tok)->token);
	free(*tok);
}
