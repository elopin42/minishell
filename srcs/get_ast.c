/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:18:48 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/06 15:20:23 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_not_operand_token(char *token)
{
	return (ft_strncmp(token, ">", 2)
		&& ft_strncmp(token, "<", 2)
		&& ft_strncmp(token, "<<", 3)
		&& ft_strncmp(token, ">>", 3)
		&& ft_strncmp(token, "||", 3)
		&& ft_strncmp(token, "&&", 3)
		&& ft_strncmp(token, "|", 2));
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
		cut_chain_and_recursive_call(p, ms);
	}
}

t_ast	*get_ast(t_tokens **og_tokens, t_env *ms)
{
	t_parser	p;

	p.tokens = *og_tokens;
	p.head = p.tokens;
	p.node = init_node();
	if (!p.node)
		return (set_parse_error(ms, MALLOC_ERROR), NULL);
	if (!p.tokens)
		return (p.node);
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
