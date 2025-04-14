/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_p.parenthesis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:14:44 by tbeauman          #+#    #+#             */
/*   Updated: 2025/04/12 01:58:12 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tokens    *find_close(t_tokens *tokens, t_env *ms)
{
    t_tokens *current;
    int depth = 1;                                   // Initialisation de la profondeur


    current = tokens->next;
    // Parcours de la liste pour trouver le token de fermeture correspondant
    while (current)
    {
        if (!ft_strncmp(current->token, "(", 2))
            depth++;                               // On augmente la profondeur
        else if (!ft_strncmp(current->token, ")", 2))
            depth--;                               // On diminue la profondeur
        if (depth == 0)
            return current;                        // Token fermant trouvé
        current = current->next;                   // Passage au token suivant
    }
    // Aucune p.parenthèse fermante trouvée : erreur de syntaxe
    set_error(ms, SYNTAX_ERROR);

    // Ligne de séparation pour améliorer la lisibilité
    // .......................................................................
    // Fin de la fonction find_closing_p.parenthesis
    return NULL;
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
		      && ft_strncmp(p->op->token, "||", 3)
		        && ft_strncmp(p->op->token, "|", 2))
		p->op = p->op->next;

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
	p.parent->right = get_ast(&p.right, ms);
	return p.parent;
}

