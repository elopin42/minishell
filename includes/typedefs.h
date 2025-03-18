/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:48:42 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/18 16:49:43 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

typedef enum e_node_type
{
    NODE_CMD,
    NODE_PIPE,
    NODE_REDIR_IN,
    NODE_REDIR_OUT,
    NODE_HERE_DOC,
    NODE_APPEND_OUT,
    NODE_AND,
    NODE_OR,
    JSP
}           t_node_type;

typedef struct s_tokens
{
    char            *token;
    t_node_type     type;
    struct s_tokens *prev;
    struct s_tokens *next;
}               t_tokens;

typedef struct s_node
{
    struct s_node   *parent;
    struct s_node   *left;
    struct s_node   *right;
    t_tokens        *tokens;
}               t_node;

typedef struct s_ast
{
    t_node_type     type;
    t_tokens        *cmd;
    char            *file;
    struct s_ast    *left;
    struct s_ast    *right;
    
}               t_ast;

typedef struct s_env
{
    char        *cmd_line;
    t_tokens    *tokens;
    char        **array_tokens;
    char        **envp;
	int			pididi;
    t_ast       *ast;
}               t_env;

#endif
