/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_list_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:05:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/04/12 03:47:52 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	add_to_tokens(t_tokens **tokens, char *tmp, char *to_trim, t_token_type type)
{
	char	*tmp2;

	tmp2 = ft_strtrimv2(tmp, to_trim);
	if (!tmp2)
		return (0);
	ft_tokens_add_back(tokens, ft_new_token(tmp2, type));
	return (1);
}

bool	is_operand1(char *str)
{
	if (!ft_strncmp(str, "|", 1)
		|| !ft_strncmp(str, "<", 1)
		|| !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "(", 1)
		|| !ft_strncmp(str, ")", 1))
		return (1);
	return (0);
}

bool	is_operand2(char *str)
{
	if (!ft_strncmp(str, "<<", 2)
		|| !ft_strncmp(str, ">>", 2)
		|| !ft_strncmp(str, "&&", 2)
		|| !ft_strncmp(str, "||", 2))
		return (1);
	return (0);
}


bool	has_dquotes(char *tmp)
{
	while (*tmp)
		if (*tmp++ == '\"')
			return (1);
	return (0);
}

bool	is_dquotes(char *tmp)
{
	return (tmp[0] == '\"' && tmp[ft_strlen(tmp) - 1] == '\"');
}

bool	has_squotes(char *tmp)
{
	while (*tmp)
		if (*tmp++ == '\'')
			return (1);
	return (0);
}


bool	is_squotes(char *tmp)
{
	return (tmp[0] == '\'' && tmp[ft_strlen(tmp) - 1] == '\'');
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
	ret = (char *)malloc((ft_strlen(s1) - count_chars_to_remove(s1, set)) + 1);
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

char	*ft_append(char *a, char *b)
{
	char *ret;

	ret = (char*)malloc(ft_strlen(a) + 1 + ft_strlen(b) + 1);
	if (!ret)
		return (NULL);
	int		i = 0;
	while (a[i])
	{
		ret[i] = a[i];
		i++;
	}
	ret[i] = '\n';
	i++;
	int		j = 0;
	while (b[j])
	{
		ret[i] = b[j];
		i++;
		j++;
	}
	ret[i] = 0;
	return (ret);
}
char	*ft_append_no_nl(char *a, char *b)
{
	char *ret;

	ret = (char*)malloc(ft_strlen(a) + ft_strlen(b) + 1);
	if (!ret)
		return (NULL);
	int		i = 0;
	while (a[i])
	{
		ret[i] = a[i];
		i++;
	}
	int		j = 0;
	while (b[j])
	{
		ret[i] = b[j];
		i++;
		j++;
	}
	ret[i] = 0;
	return (ret);
}
static bool	token_stops(char *line, int i, bool in_dquotes, bool in_quotes)
{
	if ((line[i] == ' ' || is_operand2(&line[i])
						|| is_operand1(&line[i]))
				&& !in_dquotes && !in_quotes)
		return (1);
	return (0);
}

void	handle_specials(char *line, int i, t_tknz_bools *t, t_env *ms)
{
	(void)ms;
	if (line[i] == '\"' && !t->in_quotes)
		t->in_dquotes = !t->in_dquotes;
	if (line[i] == '\'' && !t->in_dquotes)
		t->in_quotes = !t->in_quotes;
	if (line[i] == '(' && !t->in_dquotes && !t->in_quotes)
		t->parenthesis_depth++;
	if (line[i] == ')' && !t->in_dquotes && !t->in_quotes)
		t->parenthesis_depth--;
	// if (line[i] == '\\' && !t->in_dquotes && !t->in_quotes)
	// {
	// 	i++;
	// 	if (line[i] == 0)
	// 		set_error(ms, SYNTAX_ERROR);
	// 	else
	// 	{
	// 		int	j = i - 2;
	// 		while (line[++j])
	// 			line[j] = line[j + 1];
	// 		i++;
	// 	}
	// }
}

int		end_of_token(char *line, int *start, t_env *ms)
{
	int		i;
	t_tknz_bools t = {0, 0, 0};
	
	i = *start;
	if (!line)
		return (-1);
	while (line[i] && line[i] == ' ')
	{
		(*start)++;
		i++;
	}
	if (is_operand2(&line[i]))
		return (i + 2);
	if (is_operand1(&line[i]))
		return (i + 1);
	while (line[i])
	{
		handle_specials(line, i, &t, ms);
		// if (line[i] == '\"' && !t.in_quotes)
		// 	t.in_dquotes = !t.in_dquotes;
		// if (line[i] == '\'' && !t.in_dquotes)
		// 	t.in_quotes = !t.in_quotes;
		// if (line[i] == '(' && !t.in_dquotes && !t.in_quotes)
		// 	t.parenthesis_depth++;
		// if (line[i] == ')' && !t.in_dquotes && !t.in_quotes)
		// 	t.parenthesis_depth--;

		if (line[i + 1] == 0 && line[i] != ' ' && !t.in_dquotes && !t.in_quotes)
			return (i + 1);
		if (token_stops(line, i, t.in_dquotes, t.in_quotes))
			return (i);
		i++;
	}
	if (t.in_quotes || t.in_dquotes || t.parenthesis_depth)
	{
		fd_printf(2, "salfdp");
		set_error(ms, SYNTAX_ERROR);
	}
	return (i + 1);
}

void	read_nl_bs(char **next, char **tmp, t_env *ms, int *i)
{
	*next = readline(">");
	if (!*next)
	{
		fd_printf(2, "Unexpected EOF while looking for matching token\n");
		ms->err_flag = SYNTAX_ERROR;
		exit_clean(ms, ms->err_flag);
	}
	*tmp = ms->cmd_line;
	ms->cmd_line = ft_append_no_nl(*tmp, *next);
	free(*tmp);
	(*i)++;
}

void	read_nl_quotes(char **next, char **tmp, t_env *ms, int *i)
{
	*next = readline(">");
	*tmp = ms->cmd_line;
	ms->cmd_line = ft_append(*tmp, *next);
	free(*tmp);
	i++;
}

void	add_token(t_env *ms, int save_count, int count_to)
{
	char	*tmp;

	tmp = ft_substr(ms->cmd_line, save_count, count_to - save_count);
	if (!tmp)
		exit_clean(ms, MALLOC_ERROR);
	if (is_dquotes(tmp) && 
		!add_to_tokens(&ms->tokens, tmp, "\"", DQUOTES))
		exit_clean(ms, MALLOC_ERROR);
	else if (is_squotes(tmp) &&
		!add_to_tokens(&ms->tokens, tmp, "\'", SQUOTES))
		exit_clean(ms, MALLOC_ERROR);
	else if (!is_dquotes(tmp) && !is_squotes(tmp) &&
		!add_to_tokens(&ms->tokens, tmp, " ", NOQUOTES))
		exit_clean(ms, MALLOC_ERROR);
	free(tmp);
	tmp = NULL;
}

void	get_list_tokens(t_env *ms)
{
	int		count_to;
	int		save_count;
	int		len_cmd;
	int		j;

	j = 0;
	save_count = 0;
	len_cmd = ft_strlen(ms->cmd_line);
	count_to = end_of_token(ms->cmd_line, &save_count, ms);
	while (save_count != -1 && save_count < len_cmd)
	{
		add_token(ms, save_count, count_to);
		j++;
		save_count = count_to;
		if (save_count < len_cmd)
		{
			count_to = end_of_token(ms->cmd_line, &save_count, ms);
			len_cmd = ft_strlen(ms->cmd_line);
		}
	}
}
