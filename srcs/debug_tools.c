/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:10:21 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/23 23:45:02 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    print_tab(char **tab)
{
	int	i;

	i = -1;
	if (!tab || tab[0] == NULL)
		fd_printf(1, "%s:%d NULL\n", __FILE__, __LINE__);
    while (tab[++i])
    {
        fd_printf(1, "%s\n", tab[i]);
    }
}
