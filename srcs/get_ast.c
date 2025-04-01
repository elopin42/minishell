/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:18:48 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/30 17:54:13 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*init_node(void)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	node->file = NULL;
	return (node);
}

t_ast	*get_ast(t_tokens *og_tokens)
{
	t_tokens	*head;
	t_tokens	*right_tokens;
	t_ast		*node;
	t_tokens	*tokens;

	tokens = dup_tokens(og_tokens);
	head = tokens;
	node = init_node();
	if (!node)
		return (NULL);
	if (!tokens)
		return (node);
	while (tokens)
	{
		if (!ft_strcmp(tokens->token, "&&") || !ft_strcmp(tokens->token, "||"))
			break ;
		tokens = tokens->next;
		;
	}
	if (!tokens)
	{
		tokens = head;
		while (tokens)
		{
			if (!ft_strcmp(tokens->token, "|"))
				break ;
			tokens = tokens->next;
		}
		if (!tokens)
		{
			tokens = head;
			while (tokens)
			{
				if (!ft_strcmp(tokens->token, ">") || !ft_strcmp(tokens->token,
						">>") || !ft_strcmp(tokens->token, "<")
					|| !ft_strcmp(tokens->token, "<<"))
					break ;
				tokens = tokens->next;
			}
			if (!tokens)
			{
				node->cmd = head;
				node->type = NODE_CMD;
			}
			else
			{
				if (!ft_strcmp(tokens->token, ">"))
					node->type = NODE_REDIR_OUT;
				else if (!ft_strcmp(tokens->token, "<"))
					node->type = NODE_REDIR_IN;
				else if (!ft_strcmp(tokens->token, "<<"))
					node->type = NODE_HERE_DOC;
				else if (!ft_strcmp(tokens->token, ">>"))
					node->type = NODE_APPEND_OUT;
				right_tokens = tokens->next;
				if (right_tokens)
				{
					node->file = ft_strdup(right_tokens->token);
					right_tokens->prev = NULL;
				}
				if (tokens->prev)
					(tokens->prev)->next = NULL;
				node->left = get_ast(head);
				node->right = NULL;
			}
		}
		else
		{
			node->type = NODE_PIPE;
			right_tokens = tokens->next;
			if (right_tokens)
				right_tokens->prev = NULL;
			if (tokens->prev)
				(tokens->prev)->next = NULL;
			node->left = get_ast(head);
			node->right = get_ast(right_tokens);
		}
	}
	else
	{
		if (!ft_strcmp(tokens->token, "&&"))
			node->type = NODE_AND;
		else
			node->type = NODE_OR;
		right_tokens = tokens->next;
		if (right_tokens)
			right_tokens->prev = NULL;
		if (tokens->prev)
			(tokens->prev)->next = NULL;
		node->left = get_ast(head);
		node->right = get_ast(right_tokens);
	}
	return (node);
}

/*
t_ast	*get_ast(char *tokens, int *i)
{
	t_ast	*node;
		int start;
		int size;
		int j;

	if (!tokens || !tokens[*i])
		return (NULL);
	node = init_node();
	if (!node)
		return (NULL);
	if (ft_strcmp(tokens[*i], "|") == 0)
	{
		node->type = NODE_PIPE;
		(*i)++;
		node->left = get_ast(tokens, i);
		node->right = get_ast(tokens, i);
	}
	else if (!ft_strcmp(tokens[*i], ">") || !ft_strcmp(tokens[*i], "<")
		|| !ft_strncmp(tokens[*i], ">>", 2) || !ft_strncmp(tokens[*i], "<<", 2))
	{
		if (!ft_strcmp(tokens[*i], "<"))
			node->type = NODE_REDIR_IN;
		if (!ft_strcmp(tokens[*i], ">"))
			node->type = NODE_REDIR_OUT;
		if (!ft_strncmp(tokens[*i], "<<", 2))
			node->type = NODE_APPEND_IN;
		if (!ft_strncmp(tokens[*i], ">>", 2))
			node->type = NODE_APPEND_OUT;
		(*i)++;
		if (tokens[*i])
		{
			node->file = ft_strdup(tokens[*i]);
			if (!node->file)
			{
				free(node);
				return (NULL);
			}
			(*i)++;
		}
	}
	else
	{
		start = *i;
		node->type = NODE_CMD;
		while (tokens[*i] && ft_strcmp(tokens[*i], "|") && ft_strcmp(tokens[*i],
				">") && ft_strcmp(tokens[*i], "<") && ft_strcmp(tokens[*i],
				">>") && ft_strcmp(tokens[*i], "<<"))
			(*i)++;
		size = *i - start;
		node->args = (char **)malloc(sizeof(char *) * (size + 1));
		if (!node->args)
		{
			free(node);
			return (NULL);
		}
		j = 0;
		while (j < size)
		{
			node->args[j] = ft_strdup(tokens[start + j]);
			if (!node->args[j])
			{
				free_tab(node->args);
				free(node);
				return (NULL);
			}
			j++;
		}
		node->args[size] = NULL;
	}
	return (node);
}
*/