/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in-main-system.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:57:13 by elopin            #+#    #+#             */
/*   Updated: 2025/06/10 22:52:26 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdbool.h>
#include <stdio.h>

void	ft_print_echo(char *s1, char *s2, int i)
{
	if (i)
	{
		if (s2)
			printf("%s ", s1);
		else
			printf("%s\n", s1);
	}
	else if (!i)
	{
		if (s2)
			printf("%s ", s1);
		else
			printf("%s", s1);
	}
}

bool	is_valid_n_flag(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (false);
	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

bool ft_echo(t_tokens *cmd, t_tokens *t, t_env *ms)
{
    int newline;
    
    newline = 1;
    
    // Utiliser check_builtin au lieu de ft_strcmp direct
    if (check_builtin(cmd, "echo") && expand_command(&cmd, ms))
    {
        t = cmd->next;
        while (t && t->token && is_valid_n_flag(t->token))
        {
            newline = 0;
            t = t->next;
        }
        if (!t)
        {
            if (newline)
                fd_printf(1, "\n");
            return (true);
        }
        while (t)
        {
            if (t->token)
            {
                if (t->next && t->next->token)
                    ft_print_echo(t->token, t->next->token, newline);
                else
                    ft_print_echo(t->token, NULL, newline);
            }
            t = t->next;
        }
        return (true);
    }
    return (false);
}

bool	ft_cd_sys(t_tokens *t, t_env *ms)
{
	char	*tmp;
	char	*str;

	str = NULL;
	tmp = print_path(0);
	if (t && t->next)
		return (free(tmp), ms->last_exit_code = 1, true);

	str = ft_strjoin("OLDPWD=", tmp);
	free(tmp);
	ft_export(ms, str);
	free(str);

	if (t)
		ft_cd(t->token, ms);
	else
		ft_cd(NULL, ms);

	if (ms->last_exit_code == 0)
	{
		tmp = print_path(0);
		str = ft_strjoin("PWD=", tmp);
		free(tmp);
		ft_export(ms, str);
		free(str);
	}
	return (true);
}

bool	ft_export_extra(t_tokens *t, t_env *ms, int i)
{
	if (i)
	{
		while (t)
		{
			ft_unset_co(t->token, ms);
			t = t->next;
		}
		return (true);
	}
	else
	{
		if (t && t->token)
		{
			if (!t)
				return (true);
			while (t)
			{
				ft_export(ms, t->token);
				t = t->next;
			}
			return (true);
		}
		else
			return (print_export(ms), ms->last_exit_code = 0, true);
	}
}

char *remove_quotes(char *str)
{
    char *result;
    int i, j;
    
    if (!str)
        return (NULL);
    
    result = malloc(strlen(str) + 1);
    if (!result)
        return (NULL);
    
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] != '\'' && str[i] != '"')
        {
            result[j] = str[i];
            j++;
        }
        i++;
    }
    result[j] = '\0';
    return (result);
}

bool has_quotes(char *str)
{
    int i;
    
    if (!str)
        return (false);
    
    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
            return (true);
        i++;
    }
    return (false);
}

char *reconstruct_from_parts(t_token_part *parts)
{
    t_token_part *current;
    char *result;
    int total_len;
    int pos;
    
    if (!parts)
        return (NULL);
    
    // Calculer la longueur totale nécessaire
    total_len = 0;
    current = parts;
    while (current)
    {
        if (current->content)
            total_len += ft_strlen(current->content);
        current = current->next;
    }
    
    // Allouer
    result = malloc(total_len + 1);
    if (!result)
        return (NULL);
    
    // Reconstruire avec strlcpy
    result[0] = '\0';  // Initialiser à chaîne vide
    current = parts;
    while (current)
    {
        if (current->content)
        {
            // Concaténer en utilisant strlcpy
            pos = ft_strlen(result);
            ft_strlcpy(result + pos, current->content, total_len + 1 - pos);
        }
        current = current->next;
    }
    
    return (result);
}

bool check_builtin(t_tokens *token, char *builtin)
{
    char *reconstructed;
   
    if (!token || !builtin)
        return (false);
        
    if (token->token)
    {
        if (!ft_strcmp(token->token, builtin))
            return (true);
        else
            return (false);
    }
    
    reconstructed = reconstruct_from_parts(token->parts);
    if (!reconstructed)
        return (false);
    
    if (!ft_strcmp(reconstructed, builtin))
    {
        free(reconstructed);
        return (true);
    }
    
    free(reconstructed);
    return (false);
}

bool found_builtin(t_tokens *cmd, t_env *ms)
{
    t_tokens *t;
    char     *tmp;
    
    if (!cmd)
        return (false);
        
    t = cmd->next;
    
    if (check_builtin(cmd, "echo"))
        return (ft_echo(cmd, t, ms), ms->last_exit_code = 0, true);
        
    if (check_builtin(cmd, "cd") && expand_command(&cmd, ms))
        return (ft_cd_sys(t, ms), true);
        
    if (check_builtin(cmd, "pwd") && expand_command(&cmd, ms))
        return (tmp = print_path(1), free(tmp), ms->last_exit_code = 0, true);
        
    if (check_builtin(cmd, "env") && expand_command(&cmd, ms))
        return (ft_env(ms), ms->last_exit_code = 0, true);
        
    if (check_builtin(cmd, "unset") && expand_command(&cmd, ms))
        return (ft_export_extra(t, ms, 1), true);
        
    if (check_builtin(cmd, "exit"))
        return (ft_exit(t, ms), true);
        
    if (check_builtin(cmd, "export") && expand_command(&cmd, ms))
        return (ft_export_extra(t, ms, 0), true);
        
    return (false);
}

