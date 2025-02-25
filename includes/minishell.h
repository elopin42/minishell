/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:12:09 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/25 03:29:33 by elopin           ###   ########.fr       */
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

void	get_tokens(t_env *ms);
void    print_tab(char **tab);
void	handler(int	sig);
void setup_signals(void);
void	choos_command(t_env *ms);
void 	free_tab(t_env *ms);

#endif
