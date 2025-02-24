/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:05:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/23 23:43:42 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int     count_tokens(char *line)
{
    int     ret;
    int     i;
    bool     in_quotes;

    in_quotes = 0;
    i = 0;
    ret = 0;
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

int     string_to_tokens(char *line, int save_count)
{
    int     i;
    bool     in_quotes;

	i = save_count;
	in_quotes = 0;
    while (line[i] && line[i] == ' ')
		i++;
    while(line[i])
    {
        if (line[i + 1] == 0 && line[i] != ' ')
            break ;
        if (line[i] == '\"')
            in_quotes = !in_quotes;
        if (line[i] == ' ' && !in_quotes)
			break ;
		i++;
    }
	return (i + 1);
}

void	get_tokens(t_env *ms)
{
	int	count_to;
	int	save_count;
	int	i;
	int	j;

	j = 0;
	i = count_tokens(ms->cmd_line);
	save_count = 0;
	count_to = string_to_tokens(ms->cmd_line, save_count);
	ms->tokens = ft_calloc(i + 1, sizeof(char *));
	while(--i >= 0)
	{
		ms->tokens[j++] = ft_substr(ms->cmd_line, save_count, count_to - save_count);
		/*if (ms->tokens[i])
			fd_printf(1,	"token --> %s \n", ms->tokens[i]);*/
		save_count = count_to;
		count_to = string_to_tokens(ms->cmd_line, save_count);
	}
}
