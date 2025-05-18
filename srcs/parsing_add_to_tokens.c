/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_add_to_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:02:18 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/04 19:26:56 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int     add_quotes_to_tokens(t_tokens **tokens, char *tmp, char *to_trim,
    t_token_type type)
{
	char	*tmp2;

	tmp2 = ft_strtrimv2(tmp, to_trim);
	if (!tmp2)
		return (0);
	ft_tokens_add_back(tokens, ft_new_token(tmp2, type));
	return (1);

}

static char *special_trim(char *tmp, int start, int end)
{
    char    *ret;
    int     i;
    int     j;

    i = 0;
    j = 0;
    ret = (char*)malloc(sizeof(char) * (ft_strlen(tmp) - 2));
    if (!ret)
        return (NULL);
    while (tmp[i] && j != start)
    {
        ret[j] = tmp[i];
        j++;
        i++;
    }
    i++;
    while (tmp[i] && j != end)
    {
        ret[j] = tmp[i];
        j++;
        i++;
    }
    i++;
    while (tmp[i])
    {
        ret[j] = tmp[i];
        j++;
        i++;
    }
    ret[j] = 0;
    return (ret);
}

int     add_no_quotes_to_tokens(t_tokens **tokens, char *tmp)
{
    char    *tmp2;
    int     i;
    int    to_trim_start;
    int    to_trim_end = 0;
    t_token_type    type;


    i = 0;
    while (tmp[i])
    {
        if (tmp[i] == '\"' || tmp[i] == '\'')
        {
            to_trim_start = i;
            break ;
        }
        i++;
    }
    if (!tmp[i])
    {
        tmp2 = ft_strdup(tmp);
        if (!tmp2)
            return (0);
        type = NOQUOTES;
        ft_tokens_add_back(tokens, ft_new_token(tmp2, type));
        return (1);
    }
    else
    {
        i = ft_strlen(tmp);
        while (1)
        {
            if (tmp[i] == tmp[to_trim_start])
            {
                to_trim_end = i - 1;
                break ;
            }
            i--;
        }
        if (tmp[to_trim_start] == '\'')
            type = SQUOTES;
        else
            type = DQUOTES;
        tmp2 = special_trim(tmp, to_trim_start, to_trim_end);
        if (!tmp2)
            return (0);
        ft_tokens_add_back(tokens, ft_new_token(tmp2, type));
        return (1);
        }
}