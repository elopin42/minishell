/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:05:36 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 20:59:36 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	set_error(t_env *ms, int err_code)
{
	ms->last_exit_code = err_code;
	return (true);
}

bool	set_parse_error(t_env *ms, int err_code)
{
	ms->parse_error = err_code;
	return (true);
}
