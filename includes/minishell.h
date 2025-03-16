/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:12:09 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/16 18:41:07 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../fd_printf/fd_printf.h"
# include "../libft/libft.h"
# include "typedefs.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

void	get_tokens(t_env *ms);
void    print_tab(char **tab);
void	handler(int	sig);
void setup_signals(void);
void	choose_command(t_env *ms);
void 	free_tab(char **tab);
void	execute(t_env *ms);
void	cleanup(t_env *ms);
void	exit_clean(t_env *ms, int exit_code);
t_ast   *get_ast(char **tokens, int *i);
void print_ast(t_ast *node, int depth);

#endif
