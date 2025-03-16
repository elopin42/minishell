/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:18:48 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/16 18:50:25 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast    *init_node()
{
    t_ast   *node;

    node = (t_ast*)malloc(sizeof(t_ast));
    if (!node)
        return (NULL);
    node->left = NULL;
    node->right = NULL;
    node->args = NULL;
    node->file = NULL;
    return (node);
}

t_ast   *get_ast(char **tokens, int *i)
{
    t_ast   *node;

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

    else if (!ft_strcmp(tokens[*i], ">") ||
    !ft_strcmp(tokens[*i], "<") ||
    !ft_strncmp(tokens[*i], ">>", 2) ||
    !ft_strncmp(tokens[*i], "<<", 2))
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
        int     start;
        int     size;
        int     j;

        start = *i;
        node->type = NODE_CMD;
        while (tokens[*i] && ft_strcmp(tokens[*i], "|") &&
    ft_strcmp(tokens[*i], ">") && ft_strcmp(tokens[*i], "<") &&
    ft_strcmp(tokens[*i], ">>") && ft_strcmp(tokens[*i], "<<"))
            (*i)++;
        size = *i - start;
        node->args = (char**)malloc(sizeof(char *) * (size + 1));
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