/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:47:39 by tbeauman          #+#    #+#             */
/*   Updated: 2025/04/11 17:15:30 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int     ft_seek_index(char *s, char c)
{
    int     r;

    r = 0;
    while (s[r])
    {
        if (s[r] == c)
            return r;
        r++;
    }
    return (-1);
}

char    *expand_token(char *og, char *var, int si, int stop)
{
    int     i;
    int     j;
    int     k;
    char    *ret;

    ret = malloc(ft_strlen(og) + ft_strlen(var) + 2 - (stop - si));
    if (!ret)
        return (NULL);
    i = 0;
    while (i < si)
    {
        ret[i] = og[i];
        i++;
    }
    k = i + stop - si;
    j = 0;
    while (var[j])
    {
        ret[i] = var[j];
        i++;
        j++;
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

void    expand_exit_code(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
    char    *strerr;

    strerr = ft_itoa(ms->err_flag);
    if (!strerr)
        exit_clean(ms, MALLOC_ERROR);
    free((*tokens)->token);
    (*tokens)->token = expand_token(*tmp, strerr, si, si + 2);
    free(*tmp);
    free(strerr);
    if (!(*tokens)->token)
        exit_clean(ms, MALLOC_ERROR);

}

void expand_zero(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
    char    *param;

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

void expand_digit_to_empty(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
    free((*tokens)->token);
    (*tokens)->token = expand_token(*tmp, "", si, si + 2);
    free(*tmp);
    if (!(*tokens)->token)
        exit_clean(ms, MALLOC_ERROR);
}

void    search_and_destroy(t_tokens **tokens, t_env *ms)
{
    char    *tmp;
    int     si;
    int     stop;
    char    *name;
    char    *var;
    int     i;
    
    tmp = ft_strdup((*tokens)->token);
    si = ft_seek_index((*tokens)->token, '$');
    stop = si;
    if ((*tokens)->token[stop + 1] == '?')
    {
        expand_exit_code(tokens, ms, si, &tmp);
        return ;
    }
    if ((*tokens)->token[stop + 1] >= '0' && (*tokens)->token[stop + 1] <= '9')
    {
        if ((*tokens)->token[stop + 1] == '0')
        {
            expand_zero(tokens, ms, si, &tmp);
        }
        else
        {
            expand_digit_to_empty(tokens, ms, si, &tmp);
        }
        return ;
    }
    while ((*tokens)->token[stop] && (*tokens)->token[stop] != ' ')
        stop++;
    name = ft_substr(tmp, si + 1, stop - 1);
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
}

void    expand_tokens(t_ast *ast, t_env *ms)
{
    t_tokens    *tokens;

    if (!ast)
        return ;
    if (ast->type == NODE_CMD)
    {
        tokens = ast->cmd;
        while (tokens)
        {
            if (tokens->type == SQUOTES)
            {
                tokens = tokens->next;
                continue ;
            }
            else if (1 + ft_seek_index(tokens->token, '$'))
                search_and_destroy(&tokens, ms);
            tokens = tokens->next;
        }
    }
    expand_tokens(ast->left, ms);
    expand_tokens(ast->right, ms);
}

void    expand_command(t_tokens **cmd, t_env *ms)
{
    t_tokens    *tokens;

    tokens = *cmd;
    while (tokens)
    {
        if (tokens->type == SQUOTES)
        {
            tokens = tokens->next;
            continue ;
        }
        else if (1 + ft_seek_index(tokens->token, '$'))
            search_and_destroy(&tokens, ms);
        tokens = tokens->next;
    }
}