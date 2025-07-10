/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_list_tools2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:00:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/13 00:18:38 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_clear_left_tokens(t_tokens **lst, void (*del)(void *))
{
	t_tokens	*tmp;

	while (*lst)
	{
		tmp = (*lst)->prev;
		del((*lst)->token);
		free(*lst);
		(*lst) = tmp;
	}
	*lst = NULL;
}

void	ft_del_token(t_tokens *lst, void (*del)(void *))
{
	if (lst)
	{
		del(lst->token);
		free(lst);
		lst = NULL;
	}
}

void	handle_no_pipe_case(t_parser *p, t_env *ms)
{
	p->tokens = p->head;
	while (p->tokens && is_not_redir(p->tokens))
		p->tokens = p->tokens->next;
	if (!p->tokens)
	{
		p->node->cmd = p->head;
		p->node->type = NODE_CMD;
		return ;
	}
	set_node_type(p, ms);
	if ((p->node->type == NODE_APPEND_OUT
			|| p->node->type == NODE_REDIR_IN
			|| p->node->type == NODE_REDIR_OUT) && p->tokens->prev == 0
		&& !p->tokens->next)
	{
		set_parse_error(ms, SYNTAX_ERROR);
		ft_clear_right_tokens(&p->tokens, &free);
		return ;
	}
	cut_chain_for_redir(p, ms);
	recursive_call_for_redir(p, ms);
}

void	handle_pipe_case(t_parser *p, t_env *ms)
{
	set_node_type(p, ms);
	if (p->node->type != NODE_CMD)
		cut_chain_and_recursive_call(p, ms);
	else
		p->node->cmd = p->head;
}
