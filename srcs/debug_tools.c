/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:10:21 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/06 17:50:07 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tab(char **tab)
{
	int	i;

	i = -1;
	if (!tab || tab[0] == NULL)
		return ;
	while (tab[++i])
	{
		fd_printf(1, "%s\n", tab[i]);
	}
}

void	print_tokens(t_tokens *tokens)
{
	fd_printf(2, "\nTOKENS=\n");
	if (!tokens)
		fd_printf(2, "(null)\n");
	while (tokens && tokens->prev)
		tokens = tokens->prev;
	while (tokens)
	{
		fd_printf(2, "token:%s\ttype:%d\n", tokens->token, tokens->type);
		tokens = tokens->next;
	}
	fd_printf(2, "=TOKENS\n\n");
}

void	print_ast(t_ast *node, int depth)
{
	t_tokens *head;

	if (!node)
		return ;

	for (int i = 0; i < depth; i++)
		fd_printf(2, "  ");

	if (node->type == NODE_CMD)
	{
		fd_printf(2, "CMD: ");
		head = node->cmd;
		while (node->cmd)
		{
			fd_printf(2, "%s ", node->cmd->token);
			node->cmd = node->cmd->next;
		}
		node->cmd = head;
		fd_printf(2, "\n");
	}
	else if (node->type == NODE_PIPE)
		fd_printf(2, "PIPE |\n");
	else if (node->type == NODE_REDIR_OUT)
		fd_printf(2, "REDIR OUT > %s\n", node->file);
	else if (node->type == NODE_REDIR_IN)
		fd_printf(2, "REDIR IN < %s\n", node->file);
	else if (node->type == NODE_HERE_DOC)
		fd_printf(2, "HERE_DOC << %s\n", node->file);
	else if (node->type == NODE_APPEND_OUT)
		fd_printf(2, "APPEND OUT >> %s\n", node->file);
	else if (node->type == NODE_AND)
		fd_printf(2, "AND &&\n");
	else if (node->type == NODE_OR)
		fd_printf(2, "OR ||\n");
	else if (node->type == UNDEFINED)
		fd_printf(2, "(null)\n");
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}

void	print_error(int err_flag)
{
	if (err_flag == 0)
		fd_printf(2, "no error\n");
	if (err_flag == 1)
		fd_printf(2, "error\n");
	if (err_flag == SYNTAX_ERROR)
		fd_printf(2, "Syntax error\n");
	if (err_flag == CMD_NOT_FOUND_ERROR)
		fd_printf(2, "Command not found\n");
	if (err_flag == 126)
		fd_printf(2, "Permission denied\n");
}