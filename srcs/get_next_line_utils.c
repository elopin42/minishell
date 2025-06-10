/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 19:48:22 by elopin            #+#    #+#             */
/*   Updated: 2025/06/09 13:23:10 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

int	ft_read(int fd, char **BUFFER)
{
	int		bytes;
	char	*tmp;

	bytes = 0;
	tmp = malloc(BUFFER_SIZE + 1);
	if (!tmp)
		return (-1);
	bytes = read(fd, tmp, BUFFER_SIZE);
	if (bytes < 0)
		return (free(tmp), -1);
	if (bytes >= 0)
		tmp[bytes] = '\0';
	(*BUFFER) = ft_strjoin2((*BUFFER), tmp);
	free(tmp);
	return (bytes);
}

int	ft_write(char **str, char *BUFFER)
{
	int	i;
	int	j;

	i = 0;
	while (BUFFER[i] != '\n' && BUFFER[i] != '\0')
		i++;
	if (BUFFER[i] == '\n')
		i++;
	j = i;
	if (i == 0)
		return (-1);
	*str = malloc(sizeof(char) * (i + 1));
	if (!(*str))
		return (-1);
	(*str)[i] = '\0';
	while (--i >= 0)
		(*str)[i] = BUFFER[i];
	return (j);
}

int	ft_strlen2(char *str)
{
	int	i;

	if (!str)
		return (-1);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin2(char *s1, char *s2)
{
	char			*str;
	unsigned long	i;

	i = 0;
	if (!s1)
		return (ft_substr2(&s2, 0, ft_strlen2(s2)));
	str = malloc(ft_strlen2(s1) + ft_strlen2(s2) + 1);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[i - ft_strlen2(s1)])
	{
		str[i] = s2[i - ft_strlen2(s1)];
		i++;
	}
	str[i] = '\0';
	free(s1);
	return (str);
}

char	*ft_substr2(char **s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	if (!*s)
		return (NULL);
	if (start >= ((unsigned int)ft_strlen2(*s)))
	{
		new_str = (char *)malloc(1);
		if (!new_str)
			return (NULL);
		new_str[0] = '\0';
		return (new_str);
	}
	if (len > ((unsigned int)ft_strlen2(*s)) - start)
		len = ((unsigned int)ft_strlen2(*s)) - start;
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	i = start;
	j = 0;
	while (i < ((unsigned int)ft_strlen2(*s)) && j < len)
		new_str[j++] = (*s)[i++];
	new_str[j] = '\0';
	return (new_str);
}
