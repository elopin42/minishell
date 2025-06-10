/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:06:00 by elopin            #+#    #+#             */
/*   Updated: 2025/06/09 13:12:21 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000000
# endif

int		ft_strlen2(char *str);
char	*ft_strjoin2(char *s1, char *s2);
int		ft_read(int fd, char **BUFFER);
int		ft_write(char **str, char *BUFFER);
char	*get_next_line(int fd);
char	*ft_substr2(char **s, unsigned int start, size_t len);
int		check_nl(char *buffer);
int		get_next_line2(int fd, char **buffer);

#endif
