/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:01:29 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 15:44:38 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*join_path(char *cmd, char **paths, t_env *ms)
{
	char	*path;
	char	*tmp;
	int		i;

	i = 0;
	path = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			exit_clean(ms, EXIT_FAILURE);
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		tmp = 0;
		if (!path)
			exit_clean(ms, EXIT_FAILURE);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		path = 0;
		i++;
	}
	free_tab(paths);
	return (0);
}

char	*get_path(char *cmd, char **envp, t_env *ms)
{
	char	**paths;
	char	*path_var;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i] || !*(envp[i] + 5))
		return (NULL);
	path_var = envp[i] + 5;
	if (!*path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		exit_clean(ms, EXIT_FAILURE);
	return (join_path(cmd, paths, ms));
}
