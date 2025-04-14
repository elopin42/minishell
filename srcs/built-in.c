/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:46:56 by elopin            #+#    #+#             */
/*   Updated: 2025/04/11 22:16:54 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

/*void	ft_ls(const char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;
	int				i;

	i = 0;
	dir = opendir(path);
	if (!dir)
	{
		perror("ls");
		return ;
	}
	while (!i)
	{
		entry = readdir(dir);
		if (entry == NULL)
			i = 1;
		if (!i && entry->d_name[0] != '.')
			printf("%s  ", entry->d_name);
	}
	printf("\n");
	closedir(dir);
}*/
int	no_find_key(char *env)
{
	int	i;

	i = 0;

	while (env[i])
	{
		if (env[i] == '=')
			return (++i);
		i++;
	}
	return (-1);
}

void	ft_env(t_env *ms)
{
	int i;

	i = -1;
	while (ms->envp[++i])
		printf("%s\n", ms->envp[i]);
}



char	*print_path(int	a)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path && a)
		printf("%s\n", path);
	else
		perror("getcwd");
	return (path);
}

void	ft_unset_co(char *str, t_env *ms)
{
	int	i;
	int	j;

	j = 0;
	i = -1;
	while (ms->envp[++i])
	{
		if (!ft_strncmp(ms->envp[i], str, ft_strlen(str))
			&& (ms->envp[i][ft_strlen(str)] == '=' || ms->envp[i][ft_strlen(str)] == '\0'))
		{
			j = i - 1;
			while (ms->envp[++j])
				ms->envp[j] = ms->envp[j + 1];
			ms->envp[j] = NULL;
			return ;
		}
	}
}

void	ft_cd(char *path)
{
	if (!path || !*path)
		path = getenv("HOME");
	if (chdir(path) == -1)
		perror("cd");
}

int	find_index(char *str, char c)
{
	int	i;

	i = -1;
	if (!str)
		return (-1);
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (i);
}



int	find_key(char **env, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	if (!env || key_len <= 0)
		return (-1);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && (env[i][key_len] == '=' || env[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

void	update_export(t_env *ms, char *var, int h)
{
	char	*new_entry;

	new_entry = NULL;
	if (h == -1)
		return ;
	new_entry = ft_strdup(var);
	if (!new_entry)
		return ;
	printf("new_entry -> %s // h -> %d\n", new_entry, h);
	if (ms->envp[h])
		free(ms->envp[h]);
	ms->envp[h] = new_entry;
	printf("ms-envp --> %s\n", ms->envp[h]);
}

void	creat_export(t_env *ms, char *var)
{
	int		size_env;
	char	**new_env;
	int		i;

	i = -1;
	size_env = 0;
	while (ms->envp[size_env])
		size_env++;
	new_env = ft_calloc((size_env + 2), sizeof(char *));
	if (!new_env)
		return ;
	while (++i < size_env)
		new_env[i] = ms->envp[i];
	new_env[size_env] = ft_strdup(var);
	new_env[size_env + 1] = NULL;
	free(ms->envp);
	ms->envp = new_env;
}

char *add_guillemet(char *envp)
{
	int	i;
	char *str;
	char *tmps;
	char *tmp;

	i = 0;
	str = NULL;
	tmps = NULL;
	tmp = NULL;
	str = ft_substr(envp, 0, no_find_key(envp));
	tmp = ft_strjoin(str, "\"");
	free(str);
	tmps = ft_substr(envp, no_find_key(envp), ft_strlen(envp));
	str = ft_strjoin(tmp, tmps);
	free(tmp);
	free(tmps);
	tmp = ft_strjoin(str, "\"");
	free(str);
	return (tmp);
}

void ft_trie_env(t_env *ms)
{
	int	i;
	char *tmp;

	i = 0;
	tmp = NULL;
	while(ms->envp[i] && ms->envp[i + 1])
	{
		if (ft_strcmp(ms->envp[i], ms->envp[i + 1]) > 0)
		{
			tmp = ms->envp[i];
			ms->envp[i] = ms->envp[i + 1];
			ms->envp[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
}

void	print_export(t_env *ms)
{
	t_env *tmp = ms;
	int	i;
	char *str;

	i = 0;
	str = NULL;
	ft_trie_env(ms);
	while (tmp->envp[i] != NULL)
	{
		if (ft_strchr(tmp->envp[i], '='))
			str = add_guillemet(tmp->envp[i]);
		else
		 	str = ft_strdup(tmp->envp[i]);
		printf("declare -x %s\n", str);
		free(str);
		i++;
	}
}

void	ft_export(t_env *ms, char *var)
{
	char	*key;
	int		h;

	printf("var -> %s\n", var);
	if (ft_strchr(var, '&'))
	{
		printf("export: `%s': not a valid identifier", var);
		return ;
	}
	key = ft_substr(var, 0, find_index(var, '='));
	h = find_key(ms->envp, key);
	printf("h -> %d\n", h);
	free(key);
	if (h >= 0)
		update_export(ms, var, h);
	else
		creat_export(ms, var);
}

/*bool	choose_command(t_ast *ast, t_env *ms)
{
	t_tokens	*t;
	t_tokens	*cmd;
	pid_t		pid;

	if (ast->cmd == NODE_CMD)
	{
		fd_printf(2, "%d %s\n", __LINE__, __FILE__);
		print_ast(ast, 0);
		cmd = ast->cmd;
		print_tokens(cmd);
		if (cmd)
			t = cmd->next;
		else
			return (false);
		fd_printf(2, "%d %s\n", __LINE__, __FILE__);
		if (!ft_strcmp(cmd->token, "echo") && cmd->next
			&& !ft_strcmp(cmd->next->token, "-n"))
		{
			t = t->next;
			while (t)
			{
				if (t->next)
					printf("%s ", t->token);
				else
					printf("%s", t->token);
				t = t->next;
			}
			return (0);
		}
		else if (!ft_strcmp(cmd->token, "echo"))
		{
			while (t)
			{
				if (t->next)
					fd_printf(1, "%s ", t->token);
				else
					fd_printf(1, "%s\n", t->token);
				t = t->next;
			}
			return (true);
		}
		if (!ft_strcmp(cmd->token, "cd"))
		{
			if (t)
				ft_cd(t->token);
			else
				ft_cd(NULL);
			return (true);
		}
		if (!ft_strcmp(cmd->token, "pwd"))
			return (print_path(), true);
		if (!ft_strcmp(cmd->token, "env"))
			return (ft_env(ms), true);
		if (!ft_strcmp(cmd->token, "unset") && t)
			if (t->token)
				return (ft_unset_co(t->token, ms), true);
		if (!ft_strcmp(cmd->token, "exit"))
		{
			fd_printf(1, "exit\n");
			exit_clean(ms, true);
		}
		if (!ft_strcmp(cmd->token, "export") && t->token)
			return (ft_export(ms, t->token), true);
		pid = fork();
		ms->pididi = pid;
		if (pid == -1)
		{
			perror("fork");
			exit_clean(ms, EXIT_FAILURE);
		}
		if (pid == 0)
			execute_cmd(cmd, ms);
		else
		{
			wait(NULL);
			// fd_printf(2, "%d %s\n", __LINE__, __FILE__);
		}
		return (1);
	}
	return (0);
}*/

void	ft_shlvl(t_env *ms)
{
	char *str;
	int	i;
	char *tmp;
	int	b;

	b = 0;
	tmp = NULL;
	i = find_key(ms->envp, "SHLVL");
	str = NULL;
	if (i < 0)
		ft_export(ms, "SHLVL=1");
	else
	{
		str = ft_substr(ms->envp[i], find_index(ms->envp[i], '=') + 1, ft_strlen(ms->envp[i]));
		printf("str --> %s\n", str);
		b = ft_atoi(str) + 1;
		free(str);
		tmp = ft_itoa(b);
		str = ft_strjoin("SHLVL=", tmp);
		free(tmp);
		ft_export(ms, str);
		free(str);
	}
}

void ft_old_and_pwd(t_env *ms)
{
	char *str;
	char *tmp;
	str = print_path(0);
	tmp = NULL;
	tmp = ft_strjoin("PWD=", str);
	if (find_key(ms->envp, "OLDPWD") < 0)
		ft_export(ms, "OLDPWD");
	ft_export(ms, tmp);
	free(tmp);
	free(str);
	ft_shlvl(ms);
}

bool	found_builtin(t_tokens *cmd, t_env *ms)
{
	t_tokens	*t;
	char *str;
	char *tmp;

	str = NULL;
	tmp = NULL;
	expand_command(&cmd, ms);
	if (cmd)
		t = cmd->next;
	else
		return (false);
	if (!ft_strcmp(cmd->token, "echo") && cmd->next
		&& !ft_strcmp(cmd->next->token, "-n"))
	{
		t = t->next;
		while (t)
		{
			if (t->next)
				printf("%s ", t->token);
			else
				printf("%s", t->token);
			t = t->next;
		}
		return (true);
	}
	else if (!ft_strcmp(cmd->token, "echo"))
	{
		while (t)
		{
			if (t->next)
				fd_printf(1, "%s ", t->token);
			else
				fd_printf(1, "%s\n", t->token);
			t = t->next;
		}
		return (true);
	}
	if (!ft_strcmp(cmd->token, "cd"))
	{
		tmp = print_path(0);
		str = ft_strjoin("OLDPWD=", tmp);
		free(tmp);
		ft_export(ms, str);
		free(str);
		if (t)
			ft_cd(t->token);
		else
			ft_cd(NULL);
		tmp = print_path(0);
		str = ft_strjoin("PWD=", tmp);
		free(tmp);
		return (ft_export(ms, str), free(str), true);
	}
	if (!ft_strcmp(cmd->token, "pwd"))
		return (tmp = print_path(1), free(tmp), true);
	if (!ft_strcmp(cmd->token, "env"))
		return (ft_env(ms), true);
	if (!ft_strcmp(cmd->token, "unset") && t)
		if (t && t->token)
		{
			while(t)
			{
				ft_unset_co(t->token, ms);
				t = t->next;
			}
			return (true);
		}
	/*    if (!ft_strcmp(cmd->token, "ls"))
			return (ft_ls("."), true);*/
	// ls non built-in
	if (!ft_strcmp(cmd->token, "exit"))
	{
		fd_printf(1, "exit\n");
		exit_clean(ms, true);
	}
	if (!ft_strcmp(cmd->token, "export"))
	{
		if (t && t->token)
		{
			while(t)
			{
				ft_export(ms, t->token);
				t = t->next;
			}
			return(true);
		}
		else
			return (print_export(ms), true);
	}
	return (false);
}
