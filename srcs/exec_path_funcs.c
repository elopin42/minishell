/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:01:29 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/09 20:31:38 by elopin           ###   ########.fr       */
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

/*char	*get_path(char *cmd, char **envp, t_env *ms)
{
	char	**paths;
	int		i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	if (!envp[i])
		exit_clean(ms, EXIT_FAILURE);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		exit_clean(ms, EXIT_FAILURE);
	return (join_path(cmd, paths, ms));
}*/

char	*get_path(char *cmd, char **envp, t_env *ms)
{
	char	**paths;
	char	*path_var;
	int		i;
	
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i] || !*(envp[i] + 5))
		return (NULL); // PATH is unset or empty
	path_var = envp[i] + 5;
	if (!*path_var)
		return (NULL); // PATH is empty
	paths = ft_split(path_var, ':');
	if (!paths)
		exit_clean(ms, EXIT_FAILURE);
	return (join_path(cmd, paths, ms));
}

/*char	*get_path(char *cmd, char **envp, t_env *ms)
{
	char	**paths;
	char	*path_var;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;

	if (!envp[i] || !*(envp[i] + 5))
		return (NULL); // PATH est unset

	path_var = envp[i] + 5;
	if (!*path_var)
		return (NULL); // PATH est vide

	paths = ft_split(path_var, ':');
	if (!paths)
		exit_clean(ms, EXIT_FAILURE);

	return (join_path(cmd, paths, ms));
}*/

