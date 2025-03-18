/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:05:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/18 18:09:52 by tbeauman         ###   ########.fr       */
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

bool	is_operand(char *str)
{
	if (!ft_strncmp(str, "|", 1) ||
	!ft_strncmp(str, "<<", 2) ||
	!ft_strncmp(str, ">>", 2) ||
	!ft_strncmp(str, "<", 1) ||
	!ft_strncmp(str, ">", 1) ||
	!ft_strncmp(str, "&&", 2) ||
	!ft_strncmp(str, "||", 2))
		return (1);
	return (0);
}

int	string_to_tokens(char *line, int save_count)
{
	int		i;
	bool	in_quotes;
	bool	in_dquotes;

	i = save_count;
	in_quotes = 0;
	in_dquotes = 0;
	if (!line)
		return (0);
	while (line[i] && line[i] == ' ')
		i++;
	while (line[i])
	{
		if (line[i + 1] == 0 && line[i] != ' ')
			break ;
		if (line[i] == '\"' && !in_quotes)
			in_dquotes = !in_dquotes;
		if (line[i] == '\'' && !in_dquotes)
			in_quotes = !in_quotes;
		if ((line[i] == ' ' || is_operand(&line[i])) && !in_dquotes && !in_quotes)
			break ;
		i++;
	}
	return (i + 1);
}

bool	has_dquotes(char *tmp)
{
	while (*tmp)
		if (*tmp++ == '\"')
			return (1);
	return (0);
}

bool	has_squotes(char *tmp)
{
	while (*tmp)
		if (*tmp++ == '\'')
			return (1);
	return (0);
}

int	is_in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	count_chars_to_remove(char const *s1, char const *set)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (s1[i])
	{
		if (is_in_set(s1[i], set))
			ret++;
		i++;
	}
	return (ret);
}

char	*ft_strtrimv2(char const *s1, char const *set)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = (char *)malloc(ft_strlen(s1) - count_chars_to_remove(s1, set));
	if (!ret)
		return (0);
	while (s1[j])
	{
		if (!is_in_set(s1[j], set))
		{
			ret[i] = s1[j];
			i++;
		}
		j++;
	}
	ret[i] = 0;
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
	while (--i >= 0)
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
