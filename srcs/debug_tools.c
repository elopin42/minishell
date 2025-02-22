/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:10:21 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/22 23:32:59 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    print_tab(char **tab)
{
    if (!tab)
    {
        fd_printf(1, "nulltab\n");
    }
    while (*tab)
    {
        fd_printf(1, "%s\n", *tab);
        tab++;
    }
}
