/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:48:42 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/16 18:18:32 by tbeauman         ###   ########.fr       */
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
    NODE_APPEND_IN,
    NODE_APPEND_OUT
}           t_node_type;

typedef struct s_ast
{
    t_node_type     type;
    char            **args;
    char            *file;
    struct s_ast    *left;
    struct s_ast    *right;
    
}               t_ast;

typedef struct s_env
{
    char        *cmd_line;
    char        **tokens;
    char        **envp;
	int			pididi;
    t_ast       *ast;
}               t_env;

#endif
