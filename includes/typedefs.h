/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:48:42 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:34:42 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

# include <stdbool.h>

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
	UNDEFINED
}						t_node_type;

typedef enum e_token_type
{
	SQUOTES,
	DQUOTES,
	NOQUOTES,
	MULTI
}						t_token_type;

typedef enum e_quote_type
{
	NONE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}						t_quote_type;

typedef struct s_token_part
{
	char				*content;
	t_quote_type		quote;
	bool				expand;
	struct s_token_part	*next;
}						t_token_part;

typedef struct s_tokens
{
	char				*token;
	t_token_type		type;
	t_token_part		*parts;
	struct s_tokens		*prev;
	struct s_tokens		*next;
}						t_tokens;

typedef struct s_node
{
	struct s_node		*parent;
	struct s_node		*left;
	struct s_node		*right;
	t_tokens			*tokens;
}						t_node;

typedef struct s_ast
{
	t_node_type			type;
	t_tokens			*cmd;
	t_tokens			*file_token;
	char				*file;
	struct s_ast		*left;
	struct s_ast		*right;
	int					heredoc_fd;
}						t_ast;

typedef struct s_env
{
	char				*cmd_line;
	t_tokens			*tokens;
	char				**argv;
	char				**envp;
	int					pididi;
	t_ast				*ast;
	int					parse_error;
	int					last_exit_code;
	int					saved_stdin;
}						t_env;

typedef struct s_parser
{
	t_tokens			*head;
	t_tokens			*right_tokens;
	t_tokens			*left_tokens;
	t_ast				*node;
	t_tokens			*tokens;
	t_tokens			*file_token;
}						t_parser;

typedef struct s_pparser
{
	t_tokens			*open;
	t_tokens			*close;
	t_tokens			*sub_expr;
	t_tokens			*after_close;
	t_ast				*sub_ast;
	t_tokens			*op;
	t_tokens			*right;
	t_tokens			*left;
	t_ast				*parent;
}						t_pparser;

typedef struct s_token_part_args
{
	t_token_part		**parts_head;
	char				*str;
	int					start;
	int					len;
	char				quote_char;
}						t_token_part_args;

typedef struct s_quote_args
{
	t_token_part		**parts_head;
	char				*tmp;
	int					*start;
	bool				*has_quotes;
}						t_quote_args;
typedef struct s_quoted_part_args
{
	t_token_part		**parts_head;
	char				*str;
	int					start;
	int					len;
	char				quote_char;
}						t_quoted_part_args;

typedef struct s_tknz_bools
{
	bool				in_quotes;
	bool				in_dquotes;
	unsigned int		parenthesis_depth;
}						t_tknz_bools;

#endif
