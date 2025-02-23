/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:10:21 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/23 04:06:47 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    print_tab(char **tab)
{
	int	i;

	i = -1;
	if (tab || tab[0] == NULL)
		fd_printf(1, "NULL\n");
    while (tab[++i])
    {
        fd_printf(1, "%s\n", tab[i]);
    }
	fd_printf(1, "good2\n");
}
