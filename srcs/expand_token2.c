/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:16:50 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 22:18:08 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_multi(t_tokens **tokens, t_env *ms)
{
	char	*new_token;

	new_token = expand_parts((*tokens)->parts, ms);
	if (!new_token)
		exit_clean(ms, MALLOC_ERROR);
	free((*tokens)->token);
	(*tokens)->token = new_token;
}

void	expand_tokens(t_ast *ast, t_env *ms)
{
	t_tokens	*tokens;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD && ast->cmd->type == NOQUOTES)
	{
		tokens = ast->cmd;
		while (tokens)
		{
			if (tokens->type == SQUOTES)
			{
				tokens = tokens->next;
				continue ;
			}
			else if (tokens->type == MULTI && tokens->parts)
				expand_multi(&tokens, ms);
			else if (1 + ft_seek_index(tokens->token, '$') && tokens->token[1
					+ ft_seek_index(tokens->token, '$')] != 0)
				search_and_destroy(&tokens, ms);
			tokens = tokens->next;
		}
	}
	expand_tokens(ast->left, ms);
	expand_tokens(ast->right, ms);
}

void	restore_dollar(t_tokens **tokens)
{
	int	i;

	i = 0;
	while ((*tokens)->token[i])
	{
		if ((*tokens)->token[i] == 1)
			(*tokens)->token[i] = '$';
		i++;
	}
}

bool	expand_command(t_tokens **cmd, t_env *ms)
{
	t_tokens	*tokens;

	tokens = *cmd;
	while (tokens)
	{
		if (tokens->type == SQUOTES)
		{
			tokens = tokens->next;
			continue ;
		}
		if (tokens->type == MULTI)
		{
			expand_multi(&tokens, ms);
		}
		else if (1 + ft_seek_index(tokens->token, '$'))
		{
			search_and_destroy(&tokens, ms);
			restore_dollar(&tokens);
		}
		tokens = tokens->next;
	}
	return (true);
}
