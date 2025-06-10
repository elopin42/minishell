/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:47:39 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/10 22:50:13 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_seek_index(char *s, char c)
{
	int	r;

	r = 0;
	while (s[r])
	{
		if (s[r] == c)
		{
			if (s[r + 1] == 0)
				return (-1);
			return (r);
		}
		r++;
	}
	return (-1);
}

char	*expand_token(char *og, char *var, int si, int stop)
{
	int		i;
	int		j;
	int		k;
	char	*ret;

	ret = malloc(ft_strlen(og) + ft_strlen(var) + 2 - (stop - si));
	if (!ret)
		return (NULL);
	i = -1;
	while (++i < si)
		ret[i] = og[i];
	k = i + stop - si;
	j = -1;
	while (var[++j])
	{
		ret[i] = var[j];
		i++;
	}
	while (og[k])
	{
		ret[i] = og[k];
		i++;
		k++;
	}
	ret[i] = 0;
	return (ret);
}

void	expand_exit_code(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
	char	*strerr;

	strerr = ft_itoa(ms->last_exit_code);
	if (!strerr)
		exit_clean(ms, MALLOC_ERROR);
	free((*tokens)->token);
	(*tokens)->token = expand_token(*tmp, strerr, si, si + 2);
	free(*tmp);
	free(strerr);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
}

void	expand_zero(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
	char	*param;

	param = ft_strdup(ms->argv[0]);
	if (!param)
		exit_clean(ms, MALLOC_ERROR);
	free((*tokens)->token);
	(*tokens)->token = expand_token(*tmp, param, si, si + 2);
	free(*tmp);
	free(param);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
}

void	expand_digit_to_empty(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
	free((*tokens)->token);
	(*tokens)->token = expand_token(*tmp, "", si, si + 2);
	free(*tmp);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
}

bool	is_terminating_character(char c)
{
	return (c == ' ' || c == '\"' || c == '\''
			|| c == '\t' || c == '\n' || c == '|'
		|| c == '<' || c == '>' || c == 0);
}

/*void	search_and_destroy(t_tokens **tokens, t_env *ms)
{
	char	*tmp;
	int		si;
	int		stop;
	char	*name;
	char	*var;
	int		i;

	if (!*tokens || !(*tokens)->token)
		return ;
	tmp = ft_strdup((*tokens)->token);
	if (!tmp)
		return ;
	si = ft_seek_index((*tokens)->token, '$');
	if (si == -1)
		return ;
	stop = si;
	if (is_terminating_character((*tokens)->token[stop + 1]))
	{
		free((*tokens)->token);
		(*tokens)->token = expand_token(tmp, "", stop, stop + 1);  // si = stop, donc remplace le seul '$'
		free(tmp);
		if (!(*tokens)->token)
			exit_clean(ms, MALLOC_ERROR);
		if (ft_seek_index((*tokens)->token, '$') != -1)
			search_and_destroy(tokens, ms);
		return ;
	}

	if ((*tokens)->token[stop + 1] == '?')
	{
		expand_exit_code(tokens, ms, si, &tmp);
		if (1 + ft_seek_index((*tokens)->token, '$'))
			search_and_destroy(tokens, ms);
		return ;
	}
	if ((*tokens)->token[stop + 1] >= '0' && (*tokens)->token[stop + 1] <= '9')
	{
		if ((*tokens)->token[stop + 1] == '0')
			expand_zero(tokens, ms, si, &tmp);
		else
			expand_digit_to_empty(tokens, ms, si, &tmp);
		if (1 + ft_seek_index((*tokens)->token, '$'))
			search_and_destroy(tokens, ms);
		return ;
	}
	while ((*tokens)->token[stop] && !is_terminating_character(((*tokens)->token[stop])))
		stop++;
	name = ft_substr(tmp, si + 1, stop - (si + 1));
	if (!name)
	{
		free(tmp);
		exit_clean(ms, MALLOC_ERROR);
	}
	i = 0;
	while (ms->envp[i] && ft_strncmp(ms->envp[i], name, ft_strlen(name)))
		i++;
	if (!ms->envp[i])
		var = ft_strdup("");
	else
		var = ft_substr(ms->envp[i], ft_strlen(name) + 1,
				ft_strlen(ms->envp[i]));
	if (!var)
	{
		free(name);
		free(tmp);
		exit_clean(ms, MALLOC_ERROR);
	}
	free((*tokens)->token);
	(*tokens)->token = expand_token(tmp, var, si, stop);
	free(tmp);
	free(var);
	free(name);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
	if (1 + ft_seek_index((*tokens)->token, '$'))
		search_and_destroy(tokens, ms);

}*/

void	search_and_destroy(t_tokens **tokens, t_env *ms)
{
	char	*tmp;
	int		si;
	int		stop;
	char	*name;
	char	*var;
	int		i;

	if (!*tokens || !(*tokens)->token)
		return ;

	tmp = ft_strdup((*tokens)->token);
	if (!tmp)
		return ;

	si = ft_seek_index((*tokens)->token, '$');
	if (si == -1)
	{
		free(tmp);
		return ;
	}

	stop = si;

	// ➤ Si ce qui suit n'est pas un nom de variable valide : on ignore ce `$`
	char next = (*tokens)->token[stop + 1];
	if (!(next == '?' || (next >= '0' && next <= '9') || next == '_' || ft_isalpha(next)))
	{
		free(tmp);
		return ;
	}

	// ➤ Cas spécial $?
	if ((*tokens)->token[stop + 1] == '?')
	{
		expand_exit_code(tokens, ms, si, &tmp);
		if (ft_seek_index((*tokens)->token, '$') != -1)
			search_and_destroy(tokens, ms);
		return ;
	}

	// ➤ Cas $0, $1, etc.
	if ((*tokens)->token[stop + 1] >= '0' && (*tokens)->token[stop + 1] <= '9')
	{
		if ((*tokens)->token[stop + 1] == '0')
			expand_zero(tokens, ms, si, &tmp);
		else
			expand_digit_to_empty(tokens, ms, si, &tmp);
		if (ft_seek_index((*tokens)->token, '$') != -1)
			search_and_destroy(tokens, ms);
		return ;
	}

	// ➤ Cas $VAR classique
	while ((*tokens)->token[stop] && !is_terminating_character((*tokens)->token[stop]))
		stop++;

	name = ft_substr(tmp, si + 1, stop - (si + 1));
	if (!name)
	{
		free(tmp);
		exit_clean(ms, MALLOC_ERROR);
	}

	i = 0;
	while (ms->envp[i] && ft_strncmp(ms->envp[i], name, ft_strlen(name)))
		i++;

	if (!ms->envp[i])
		var = ft_strdup("");
	else
		var = ft_substr(ms->envp[i], ft_strlen(name) + 1, ft_strlen(ms->envp[i]));

	if (!var)
	{
		free(name);
		free(tmp);
		exit_clean(ms, MALLOC_ERROR);
	}

	free((*tokens)->token);
	(*tokens)->token = expand_token(tmp, var, si, stop);
	free(tmp);
	free(var);
	free(name);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);

	if (ft_seek_index((*tokens)->token, '$') != -1)
		search_and_destroy(tokens, ms);
}

char	*expand_parts(t_token_part *parts, t_env *ms)
{
	char	*final = NULL;
	char	*expanded = NULL;
	t_token_part *p = parts;
	bool all_empty = true;
	t_token_part *check = parts;
	while (check)
	{
		if (check->content && check->content[0] != '\0')
		{
			all_empty = false;
			break;
		}
		check = check->next;
	}
	if (all_empty)
		return ft_strdup("");


	while (p)
	{
		if (p->expand)
		{
			t_tokens *tmp_tok = malloc(sizeof(t_tokens));
			
			if (!tmp_tok)
				exit_clean(ms, MALLOC_ERROR);
			tmp_tok->token = ft_strdup(p->content);
			if (!tmp_tok->token)
				exit_clean(ms, MALLOC_ERROR);
			tmp_tok->next = NULL;
			tmp_tok->prev = NULL;
			tmp_tok->parts = NULL;
			tmp_tok->type = NOQUOTES;
			search_and_destroy(&tmp_tok, ms);
			expanded = ft_strdup(tmp_tok->token);
			free(tmp_tok->token);
			free(tmp_tok);

		}
		else
			expanded = ft_strdup(p->content);
		if (!expanded)
			exit_clean(ms, MALLOC_ERROR);
		char *new_final;
		if (final)
		{
			new_final = ft_strjoin(final, expanded);
			free(final);
			free(expanded);
		}
		else
		{
			new_final = ft_strjoin("", expanded);
			free(expanded);
		}
		if (!new_final)
			exit_clean(ms, MALLOC_ERROR);
		final = new_final;
		p = p->next;
	}
	return (final);
}

void	expand_multi(t_tokens **tokens, t_env *ms)
{
	char	*new_token;

	new_token = expand_parts((*tokens)->parts, ms);
	if (!new_token)
		exit_clean(ms, MALLOC_ERROR);
	free((*tokens)->token);
	(*tokens)->token = new_token;
}

void	expand_tokens(t_ast *ast, t_env *ms)
{
	t_tokens	*tokens;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD && ast->cmd->type == NOQUOTES)
	{
		tokens = ast->cmd;
		while (tokens)
		{
			if (tokens->type == SQUOTES)
			{
				tokens = tokens->next;
				continue ;
			}
			else if (tokens->type == MULTI && tokens->parts)
			{
				expand_multi(&tokens, ms);
			}
			else if (1 + ft_seek_index(tokens->token, '$')
			&& tokens->token[1 + ft_seek_index(tokens->token, '$')] != 0)
			{
				search_and_destroy(&tokens, ms);
			}
			tokens = tokens->next;
		}
	}
	expand_tokens(ast->left, ms);
	expand_tokens(ast->right, ms);
}

void	restore_dollar(t_tokens **tokens)
{
	int		i;

	i = 0;
	while ((*tokens)->token[i])
	{
		if ((*tokens)->token[i] == 1)
			(*tokens)->token[i] = '$';
		i++;
	}
}

bool	expand_command(t_tokens **cmd, t_env *ms)
{
	t_tokens *tokens;

	tokens = *cmd;
	while (tokens)
	{
		if (tokens->type == SQUOTES)
		{
			tokens = tokens->next;
			continue ;
		}
		if (tokens->type == MULTI)
		{
			expand_multi(&tokens, ms);
		}
		else if (1 + ft_seek_index(tokens->token, '$'))
		{
			search_and_destroy(&tokens, ms);
			restore_dollar(&tokens);
		}
		tokens = tokens->next;
	}
	return (true);
}
