/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parenthesis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:00:42 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 21:02:56 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tokens	*find_close(t_tokens *tokens, t_env *ms)
{
	t_tokens	*current;
	int			depth;

	depth = 1;
	current = tokens->next;
	while (current)
	{
		if (!ft_strncmp(current->token, "(", 2))
			depth++;
		else if (!ft_strncmp(current->token, ")", 2))
			depth--;
		if (depth == 0)
			return (current);
		current = current->next;
	}
	set_error(ms, SYNTAX_ERROR);
	return (NULL);
}

void	cut_chain(t_pparser *p)
{
	p->sub_expr = p->open->next;
	p->after_close = p->close->next;
	if (p->sub_expr)
		p->sub_expr->prev = NULL;
	if (p->close->prev)
		p->close->prev->next = NULL;
}

void	isolate_op(t_pparser *p)
{
	p->right = p->op->next;
	p->left = p->op->prev;
	if (p->left)
		p->left->next = NULL;
	if (p->right)
		p->right->prev = NULL;
	p->op->next = NULL;
	p->op->prev = NULL;
}

void	set_type(t_pparser *p)
{
	if (!ft_strncmp(p->op->token, "&&", 3))
		p->parent->type = NODE_AND;
	else if (!ft_strncmp(p->op->token, "||", 3))
		p->parent->type = NODE_OR;
	else if (!ft_strncmp(p->op->token, "|", 2))
		p->parent->type = NODE_PIPE;
}

void	find_next_op(t_pparser *p)
{
	p->op = p->after_close;
	while (p->op && ft_strncmp(p->op->token, "&&", 3)
		&& ft_strncmp(p->op->token, "||", 3) && ft_strncmp(p->op->token, "|",
			2))
		p->op = p->op->next;
}
