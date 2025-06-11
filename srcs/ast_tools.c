/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:17:46 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 20:42:15 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*init_node(void)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->file_token = NULL;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	node->file = NULL;
	node->type = UNDEFINED;
	return (node);
}

bool	is_not_redir(t_tokens *token)
{
	if (token->type == NOQUOTES)
		return (ft_strncmp(token->token, ">", 2) && ft_strncmp(token->token,
				"<", 2) && ft_strncmp(token->token, ">>", 3)
			&& ft_strncmp(token->token, "<<", 3));
	return (1);
}

void	cut_chain_and_recursive_call(t_parser *p, t_env *ms)
{
	p->right_tokens = p->tokens->next;
	p->left_tokens = p->tokens->prev;
	if (p->right_tokens)
		p->right_tokens->prev = NULL;
	if (p->left_tokens)
		p->left_tokens->next = NULL;
	else
		p->head = NULL;
	p->node->cmd = p->tokens;
	p->node->cmd->prev = NULL;
	p->node->cmd->next = NULL;
	p->node->left = get_ast(&p->head, ms);
	p->node->right = get_ast(&p->right_tokens, ms);
}

t_tokens	*dup_token(t_tokens *token)
{
	char			*tmp;
	t_token_part	*parts;
	t_tokens		*ret;

	tmp = ft_strdup(token->token);
	if (!tmp)
		return (NULL);
	if (token->type == MULTI)
	{
		parts = dup_parts(token->parts);
		if (!parts)
		{
			free(tmp);
			return (NULL);
		}
	}
	else
		parts = NULL;
	ret = ft_new_token(tmp, token->type, parts);
	return (ret);
}

void	append_arguments(t_parser *p, t_env *ms)
{
	t_tokens	*token_tmp;
	t_tokens	*new_tok;

	while (p->right_tokens && !is_logic_operator(p->right_tokens->token))
	{
		new_tok = dup_token(p->right_tokens);
		if (!new_tok)
		{
			exit_clean(ms, MALLOC_ERROR);
		}
		ft_tokens_add_back(&p->left_tokens, new_tok);
		token_tmp = p->right_tokens;
		p->right_tokens = p->right_tokens->next;
		free_token(&token_tmp);
		token_tmp = NULL;
		if (p->right_tokens)
			p->right_tokens->prev = NULL;
	}
}
