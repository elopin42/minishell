/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:05:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/21 09:26:54 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     count_tokens(char *line)
{
    int     ret;
    int     i;
    bool     in_quotes;

    in_quotes = 0;
    i = 0;
    ret = 0;
    if (line[i] == ' ')
        while (line[i] && line[i] == ' ')
            i++;
    while(line[i])
    {
        if (line[i + 1] == 0 && line[i] != ' ')
            ret++;
        if (line[i] == '\"')
            in_quotes = !in_quotes;
        if (line[i] == ' ' && !in_quotes)
        {
            ret++;
            while (line[i + 1] == ' ')
                i++;
        }
        i++;
    }
    return (ret);
}

char    **get_tokens(t_env *ms)
{
    if ()
}