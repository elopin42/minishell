/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_by_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:02:51 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/10 23:28:26 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../includes/minishell.h" // ou adapte à tes fonctions

t_token_part	*new_token_part(char *content, t_quote_type quote, bool expand)
{
	t_token_part *part = malloc(sizeof(t_token_part));
	if (!part)
		return (NULL);
	part->content = content;
	part->quote = quote;
	part->expand = expand;
	part->next = NULL;
	return (part);
}

void	append_token_part(t_token_part **head, t_token_part *new_part)
{
	t_token_part *tmp = *head;
	if (!*head)
	{
		*head = new_part;
		return;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_part;
}

t_token_part    *dup_parts(t_token_part *parts)
{
    t_token_part    *ret;
    t_token_part    *new_part;
    char            *new;

    ret = NULL;
    while (parts)
    {
        new = ft_strdup(parts->content);
        if (!new)
        {
            free_token_parts(&ret);
            return (NULL);
        }
        new_part = new_token_part(new, parts->quote, parts->expand);
        if (!new_part)
        {
            free_token_parts(&ret);
            return (NULL);
        }
        append_token_part(&ret, new_part);
        parts = parts->next;
    }
    return (ret);
}

void	free_token_parts(t_token_part **head)
{
	t_token_part *tmp;

	while (*head)
	{
		tmp = (*head)->next;
        if ((*head)->content)
    		free((*head)->content);
        if (*head)
            free(*head);
		*head = tmp;
	}
}

/*t_token_part	*split_token_by_quotes(const char *str)
{
	t_token_part	*parts = NULL;
	int				i = 0;
	int				start;
	char			*sub;
	t_quote_type	current_quote = NONE;
	bool			expand = true;

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && strchr(&str[i + 1], str[i]))
		{
			char	quote_char = str[i];
			current_quote = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
			expand = (current_quote == DOUBLE_QUOTE);
			i++;
			start = i;
			while (str[i] && str[i] != quote_char)
				i++;
			sub = ft_substr(str, start, i - start);
			if (!sub)
				return (free_token_parts(&parts), NULL);
			append_token_part(&parts, new_token_part(sub, current_quote, expand));
			if (str[i])
				i++;
			current_quote = NONE;
			expand = true;
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '\'' && str[i] != '"')
				i++;
			if (i - start > 0)
			{
				sub = ft_substr(str, start, i - start);
				if (!sub)
				return (free_token_parts(&parts), NULL);
			append_token_part(&parts, new_token_part(sub, NONE, true));
			}
		}
	}
	return (parts);
}*/

/*t_token_part	*split_token_by_quotes(const char *str)
{
	t_token_part	*parts = NULL;
	int				i = 0;
	int				start;
	char			*sub;
	t_quote_type	current_quote;
	bool			expand;

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && strchr(&str[i + 1], str[i]))
		{
			char quote_char = str[i];
			current_quote = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
			expand = (current_quote == DOUBLE_QUOTE);
			i++; // skip opening quote
			start = i;

			while (str[i] && str[i] != quote_char)
				i++;

			if (str[i] == quote_char)
			{
				sub = ft_substr(str, (unsigned int)start, (size_t)(i - start));
				if (!sub)
					return (free_token_parts(&parts), NULL);

				// ❌ skip '' silently
				if (!(sub[0] == '\0' && current_quote == SINGLE_QUOTE))
				{
					t_token_part *new = new_token_part(sub, current_quote, expand);
					if (!new)
						return (free_token_parts(&parts), NULL);
					append_token_part(&parts, new);
				}
				else
					free(sub);

				i++; // skip closing quote
			}
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '\'' && str[i] != '"')
				i++;
			if (i - start > 0)
			{
				sub = ft_substr(str, (unsigned int)start, (size_t)(i - start));
				if (!sub)
					return (free_token_parts(&parts), NULL);
				t_token_part *new = new_token_part(sub, NONE, true);
				if (!new)
					return (free_token_parts(&parts), NULL);
				append_token_part(&parts, new);
			}
		}
	}
	return (parts);
}*/


t_token_part	*split_token_by_quotes(const char *str)
{
	t_token_part	*parts = NULL;
	int				i = 0;
	int				start;
	char			*sub;
	t_quote_type	current_quote;
	bool			expand;

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && strchr(&str[i + 1], str[i]))
		{
			char quote_char = str[i];
			current_quote = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
			expand = (current_quote == DOUBLE_QUOTE);
			i++; // skip opening quote
			start = i;

			while (str[i] && str[i] != quote_char)
				i++;

			if (str[i] == quote_char)
			{
				sub = ft_substr(str, (unsigned int)start, (size_t)(i - start));
				if (!sub)
					return (free_token_parts(&parts), NULL);

				if (!(sub[0] == '\0' && current_quote == SINGLE_QUOTE))
				{
					t_token_part *new = new_token_part(sub, current_quote, expand);
					if (!new)
						return (free_token_parts(&parts), NULL);
					append_token_part(&parts, new);
				}
				else
					free(sub);

				i++; // skip closing quote
			}
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '\'' && str[i] != '"')
				i++;
			if (i - start > 0)
			{
				sub = ft_substr(str, (unsigned int)start, (size_t)(i - start));
				if (!sub)
					return (free_token_parts(&parts), NULL);
				t_token_part *new = new_token_part(sub, NONE, true);
				if (!new)
					return (free_token_parts(&parts), NULL);
				append_token_part(&parts, new);
			}
		}
	}
	return (parts);
}

