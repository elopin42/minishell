/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:18:48 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:57:46 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_not_operand_token(char *token)
{
	return (ft_strncmp(token, ">", 2) && ft_strncmp(token, "<", 2)
		&& ft_strncmp(token, "<<", 3) && ft_strncmp(token, ">>", 3)
		&& ft_strncmp(token, "||", 3) && ft_strncmp(token, "&&", 3)
		&& ft_strncmp(token, "|", 2));
}

void	set_node_type(t_parser *p, t_env *ms)
{
	(void)ms;
	if (p->tokens->type == NOQUOTES && !ft_strncmp(p->tokens->token, ">", 2))
		p->node->type = NODE_REDIR_OUT;
	else if (p->tokens->type == NOQUOTES && !ft_strncmp(p->tokens->token, "<",
			2))
		p->node->type = NODE_REDIR_IN;
	else if (p->tokens->type == NOQUOTES && !ft_strncmp(p->tokens->token, "<<",
			3))
		p->node->type = NODE_HERE_DOC;
	else if (p->tokens->type == NOQUOTES && !ft_strncmp(p->tokens->token, ">>",
			3))
		p->node->type = NODE_APPEND_OUT;
	else if (p->tokens->type == NOQUOTES && !ft_strncmp(p->tokens->token, "&&",
			3))
		p->node->type = NODE_AND;
	else if (p->tokens->type == NOQUOTES && !ft_strncmp(p->tokens->token, "||",
			3))
		p->node->type = NODE_OR;
	else if (p->tokens->type == NOQUOTES && !ft_strncmp(p->tokens->token, "|",
			2))
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
		handle_no_pipe_case(p, ms);
	else
		handle_pipe_case(p, ms);
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
	if (p.tokens && p.tokens->token && !ft_strncmp(p.tokens->token, "(", 2))
		return (free(p.node), handle_parenthesis(p.tokens, og_tokens, ms));
	while (p.tokens && p.tokens->token && ft_strncmp(p.tokens->token, "&&", 3)
		&& ft_strncmp(p.tokens->token, "||", 3))
		p.tokens = p.tokens->next;
	if (!p.tokens)
		no_logic_parser(&p, ms);
	else
	{
		set_node_type(&p, ms);
		if (p.node->type != NODE_CMD)
			cut_chain_and_recursive_call(&p, ms);
		else
			p.node->cmd = p.head;
	}
	return (p.node);
}

bool	is_valid_expand_char(char c)
{
	return (c == '?' || (c >= '0' && c <= '9') || c == '_' || ft_isalpha(c));
}
