/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_list_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:05:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/09 15:17:54 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	end_of_token(char *line, int *start, t_env *ms)
{
	int				i;
	t_tknz_bools	t;

	t = (t_tknz_bools){0, 0, 0};
	i = *start;
	if (!line)
		return (-1);
	while (line[i] && line[i] == ' ')
	{
		i++;
	}
	*start = i;
	if (line[i] == 0)
	{
		if (is_operand1(&line[i - 1]))
		{
			return (i - 1);
		}
	}
	if (is_operand(&line[i]))
		return (compute_end_operand_index(&line[i], i, start));
	while (line[i])
	{
		handle_specials(line, i, &t, ms);
		if (line[i + 1] == 0 && line[i] != ' ' && !t.in_dquotes && !t.in_quotes)
			return (i + 1);
		if (token_stops(line, i, t.in_dquotes, t.in_quotes))
			return (i);
		i++;
	}
	if (!check_quotes_ended(t, ms))
		return (-1);
	return (i + 1);
}

void	add_token(t_env *ms, int save_count, int count_to)
{
	char	*tmp;

	tmp = ft_substr(ms->cmd_line, save_count, count_to - save_count);
	if (!tmp)
		exit_clean(ms, MALLOC_ERROR);
	if (is_dquotes(tmp) && !add_quotes_to_tokens(&ms->tokens, tmp, "\"", DQUOTES))
		exit_clean(ms, MALLOC_ERROR);
	else if (is_squotes(tmp) && !add_quotes_to_tokens(&ms->tokens, tmp, "\'", SQUOTES))
		exit_clean(ms, MALLOC_ERROR);
	else if (!is_dquotes(tmp) && !is_squotes(tmp) && !add_no_quotes_to_tokens(&ms->tokens,
			tmp))
		exit_clean(ms, MALLOC_ERROR);
	free(tmp);
	tmp = NULL;
}

bool	bout_de_scotch(char *cmd_line)
{
	if (ft_strlen(cmd_line) > 1 && is_operand1(&cmd_line[ft_strlen(cmd_line)
			- 1]) && cmd_line[ft_strlen(cmd_line) - 1] != ')')
		return (1);
	if (ft_strlen(cmd_line) > 2 && is_operand2(&cmd_line[ft_strlen(cmd_line)
			- 2]))
		return (1);
	return (0);
}

void	get_list_tokens(t_env *ms)
{
	int	count_to;
	int	save_count;
	int	len_cmd;
	int	j;

	j = 0;
	save_count = 0;
	len_cmd = ft_strlen(ms->cmd_line);
	count_to = end_of_token(ms->cmd_line, &save_count, ms);
	if (count_to == -1)
		return ;
	while (save_count != -1 && save_count < len_cmd)
	{
		add_token(ms, save_count, count_to);
		j++;
		save_count = count_to;
		if (save_count <= len_cmd)
		{
			count_to = end_of_token(ms->cmd_line, &save_count, ms);
			if (count_to == -1)
			{
				print_error(ms->parse_error);
				return ;
			}
		}
	}
}
