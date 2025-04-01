/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_list_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:05:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/30 17:54:17 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_list_tokens(t_env *ms)
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
	while (--i >= 0)
	{
		tmp = ft_substr(ms->cmd_line, save_count, count_to - save_count);
		if (has_dquotes(tmp))
			ft_tokens_add_back(&ms->tokens, ft_new_token(ft_strtrimv2(tmp,
						"\""), JSP));
		else if (has_squotes(tmp))
			ft_tokens_add_back(&ms->tokens, ft_new_token(ft_strtrimv2(tmp,
						"\'"), JSP));
		else
			ft_tokens_add_back(&ms->tokens, ft_new_token(ft_strtrimv2(tmp, " "),
					JSP));
		free(tmp);
		tmp = NULL;
		j++;
		save_count = count_to;
		count_to = string_to_tokens(ms->cmd_line, save_count);
	}
}
