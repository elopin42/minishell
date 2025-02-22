/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 10:49:14 by tbeauman          #+#    #+#             */
/*   Updated: 2024/11/24 11:10:02 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nb_c(int n)
{
	int	count;

	count = 1;
	if (n < 0)
		count++;
	while (n / 10 != 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

static void	ft_strrev(char *str)
{
	unsigned int	i;
	unsigned int	len;
	char			tmp;

	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		tmp = str[i];
		str[i] = str[len - 1];
		str[len - 1] = tmp;
		i++;
		len--;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		neg;
	int		i;

	neg = (n < 0);
	str = (char *)malloc(sizeof(char) * (ft_nb_c(n) + 1));
	if (!str)
		return (NULL);
	str[ft_nb_c(n)] = 0;
	if (!str)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	i = 0;
	while (n != 0)
	{
		str[i] = '0' + ft_abs(n % 10);
		n /= 10;
		i++;
	}
	if (neg)
		str[i] = '-';
	ft_strrev(str);
	return (str);
}
