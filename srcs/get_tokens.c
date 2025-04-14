/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:05:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/04/10 17:31:49 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_tokens(char *line)
{
	int		ret;
	int		i;
	bool	in_quotes;

	in_quotes = 0;
	i = 0;
	ret = 0;
	if (!line)
		return (0);
	while (line[i] && line[i] == ' ')
		i++;
	while (line[i])
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


void	get_tokens(t_env *ms)
{
	int		count_to;
	int		save_count;
	char	*tmp;
	int		i;
	int		j;

	j = 0;
	i = count_tokens(ms->cmd_line);
	save_count = 0;
	count_to = string_to_tokens(ms->cmd_line, save_count);
	ms->array_tokens = ft_calloc(i + 1, sizeof(char *));
	while (count_to < ft_strlen(ms->cmd_line))
	{
		tmp = ft_substr(ms->cmd_line, save_count, count_to - save_count);
		if (has_dquotes(tmp))
			ms->array_tokens[j] = ft_strtrimv2(tmp, "\"");
		else if (has_squotes(tmp))
			ms->array_tokens[j] = ft_strtrimv2(tmp, "\'");
		else
			ms->array_tokens[j] = ft_strtrimv2(tmp, " ");
		free(tmp);
		tmp = NULL;
		j++;
		save_count = count_to;
		count_to = string_to_tokens(ms->cmd_line, save_count);
	}
}
