/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:18:48 by tbeauman          #+#    #+#             */
/*   Updated: 2025/04/12 01:52:05 by tbeauman         ###   ########.fr       */
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

bool	is_not_redir(char *token)
{
	return (ft_strncmp(token, ">", 2) &&
	ft_strncmp(token, "<", 2) &&
	ft_strncmp(token, ">>", 3) &&
	ft_strncmp(token, "<<", 3));
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

void	cut_chain_for_redir(t_parser *p, t_env *ms)
{
	p->file_token = p->tokens->next;
	p->right_tokens = NULL;
	p->left_tokens = p->tokens->prev;
	if (p->file_token)
	{
		p->node->file = p->file_token->token;
		p->right_tokens = p->file_token->next;
		p->file_token->prev = NULL;
		p->file_token->next = NULL;
		p->node->file_token = p->file_token;				
	}
	else
		set_error(ms, SYNTAX_ERROR);
	if (p->left_tokens)
		p->left_tokens->next = NULL;
	else
		p->head = NULL;
	p->node->cmd = p->tokens;
	p->node->cmd->prev = NULL;
	p->node->cmd->next = NULL;
}

void	recursive_call_for_redir(t_parser *p, t_env *ms)
{
	t_tokens *tmp;

	p->node->left = get_ast(&p->head, ms);
	if (p->right_tokens)
	{
		tmp = p->right_tokens;
		p->right_tokens->prev->next = NULL;
		tmp->prev = NULL;
		p->node->right = get_ast(&tmp, ms);
	}
	else
		p->node->right = NULL;
}

void	set_node_type(t_parser *p, t_env *ms)
{
	(void)ms;
	if (!ft_strncmp(p->tokens->token, ">", 2))
		p->node->type = NODE_REDIR_OUT;
	else if (!ft_strncmp(p->tokens->token, "<", 2))
		p->node->type = NODE_REDIR_IN;
	else if (!ft_strncmp(p->tokens->token, "<<", 3))
		p->node->type = NODE_HERE_DOC;
	else if (!ft_strncmp(p->tokens->token, ">>", 3))
		p->node->type = NODE_APPEND_OUT;
	else if (!ft_strncmp(p->tokens->token, "&&", 3))
		p->node->type = NODE_AND;
	else if (!ft_strncmp(p->tokens->token, "||", 3))
		p->node->type = NODE_OR;
	else if (!ft_strncmp(p->tokens->token, "|", 2))
		p->node->type = NODE_PIPE;
	else
		p->node->type = NODE_CMD;
}

void	no_logic_parser(t_parser *p, t_env *ms)
{
	p->tokens = p->head;
	while (p->tokens && ft_strncmp(p->tokens->token, "|", 2))
		p->tokens = p->tokens->next;
	if (!p->tokens)
	{
		p->tokens = p->head;
		while (p->tokens && is_not_redir(p->tokens->token))
			p->tokens = p->tokens->next;
		if (!p->tokens)
		{
			p->node->cmd = p->head;
			p->node->type = NODE_CMD;
		}
		else
		{
			set_node_type(p, ms);
			cut_chain_for_redir(p, ms);
			recursive_call_for_redir(p, ms);
		}
	}
	else
	{
		set_node_type(p, ms);
		cut_chain_and_recursive_call(p,ms);
	}
}

t_ast	*get_ast(t_tokens **og_tokens, t_env *ms)
{
	t_parser	p;

	p.tokens = *og_tokens;
	p.head = p.tokens;
	p.node = init_node();
	if (!p.node)
		return (set_error(ms, MALLOC_ERROR), NULL);
	if (!p.tokens)
		return (set_error(ms, MALLOC_ERROR), p.node);
	if (p.tokens && !ft_strncmp(p.tokens->token, "(", 2))
		return (free(p.node), handle_parenthesis(p.tokens, og_tokens, ms));
	while (p.tokens && ft_strncmp(p.tokens->token, "&&", 3)
					&& ft_strncmp(p.tokens->token, "||", 3))
		p.tokens = p.tokens->next;
	if (!p.tokens)
		no_logic_parser(&p, ms);
	else
	{
		set_node_type(&p, ms);
		cut_chain_and_recursive_call(&p, ms);
	}
	return (p.node);
}
