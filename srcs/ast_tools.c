/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:17:46 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/09 17:12:25 by tbeauman         ###   ########.fr       */
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
		return (ft_strncmp(token->token, ">", 2) && ft_strncmp(token->token, "<", 2)
		&& ft_strncmp(token->token, ">>", 3) && ft_strncmp(token->token, "<<", 3));
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

void	append_arguments(t_parser *p, t_env *ms)
{
	t_tokens		*token_tmp;
	char		*tmp;
	
	while (p->right_tokens && !is_operand(p->right_tokens->token))
	{

		tmp = ft_strdup(p->right_tokens->token);
		if (!tmp)
			exit_clean(ms, 1);
		ft_tokens_add_back(&p->left_tokens, ft_new_token(tmp, p->right_tokens->type, NULL));
		token_tmp = p->right_tokens;
		p->right_tokens = p->right_tokens->next;
		free(token_tmp->token);
		free(token_tmp);
		token_tmp = NULL;
		if (p->right_tokens)
			p->right_tokens->prev = NULL;
	}

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
	append_arguments(p, ms);
	p->node->cmd = p->tokens;
	p->node->cmd->prev = NULL;
	p->node->cmd->next = NULL;
}

void	recursive_call_for_redir(t_parser *p, t_env *ms)
{
	t_tokens	*tmp;

	p->node->left = get_ast(&p->head, ms);
	if (p->right_tokens)
	{
		tmp = p->right_tokens;
		if (p->right_tokens->prev)
			p->right_tokens->prev->next = NULL;
		tmp->prev = NULL;
		p->node->right = get_ast(&tmp, ms);
	}
	else
		p->node->right = NULL;
}
