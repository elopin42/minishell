/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:15:41 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:59:54 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("SIGINT error\n");
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("SIGQUIT error\n");
	}
	sa.sa_handler = SIG_IGN;
	sigaction(SIGTSTP, &sa, NULL);
}

void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

t_ast	*handle_parenthesis(t_tokens *tokens, t_tokens **og_tokens, t_env *ms)
{
	t_pparser	p;

	p.open = tokens;
	p.close = find_close(p.open, ms);
	if (!p.close)
		return (set_error(ms, SYNTAX_ERROR), NULL);
	cut_chain(&p);
	p.sub_ast = get_ast(&p.sub_expr, ms);
	if (!p.sub_ast)
		return (NULL);
	*og_tokens = p.after_close;
	if (!p.after_close)
		return (p.sub_ast);
	find_next_op(&p);
	if (!p.op)
		return (p.sub_ast);
	isolate_op(&p);
	p.parent = init_node();
	if (!p.parent)
		return (set_error(ms, MALLOC_ERROR), NULL);
	set_type(&p);
	p.parent->cmd = p.op;
	p.parent->left = p.sub_ast;
	return (p.parent->right = get_ast(&p.right, ms), p.parent);
}

void	delete_ast(t_ast **node)
{
	if (!*node)
		return ;
	delete_ast(&((*node)->left));
	delete_ast(&((*node)->right));
	if ((*node)->cmd)
		ft_clear_tokens(&((*node)->cmd), &free);
	if ((*node)->file_token)
		ft_clear_tokens(&(*node)->file_token, &free);
	free(*node);
	*node = NULL;
}

void	print_logo(void)
{
	printf("\033[1;32m");
	printf("██╗  ██╗███████╗██╗     ██╗\n");
	printf("██║  ██║██╔════╝██║     ██║\n");
	printf("███████║█████╗  ██║     ██║\n");
	printf("██╔══██║██╔══╝  ██║     ██║\n");
	printf("██║  ██║███████╗███████╗███████╗\n");
	printf("\033[0m\n");
}
