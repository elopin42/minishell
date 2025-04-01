/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 19:06:47 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/21 08:22:46 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fd_printf.h"

int	print_c(int fd, va_list *ap)
{
	int	c;

	c = va_arg(*ap, int);
	ft_putchar(c, fd);
	return (1);
}

int	print_s(int fd, va_list *ap)
{
	char	*s;

	s = va_arg(*ap, char *);
	if (!s)
	{
		ft_putstr("(null)", fd);
		return (6);
	}
	ft_putstr(s, fd);
	return (ft_lenstr(s));
}

int	custom_putnbr_base(int fd, long p, int b)
{
	char	*base;
	int		ret;
	int		neg;

	ret = 1;
	neg = 0;
	if (b == 0 || b >= 36)
		return (-1);
	if (b == 16 && p == (long)0x8000000000000000)
	{
		ft_putstr("8000000000000000", fd);
		return (ft_lenstr("8000000000000000"));
	}
	base = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (p < 0)
	{
		p = -p;
		ft_putchar('-', fd);
		neg = 1;
	}
	if (p / b != 0)
		ret = 1 + custom_putnbr_base(fd, p / b, b);
	ft_putchar(base[p % b], fd);
	return (ret + neg);
}

int	print_p(int fd, va_list *ap)
{
	long	p;

	p = va_arg(*ap, long);
	if (!p)
	{
		ft_putstr("(nil)", fd);
		return (5);
	}
	if ((unsigned long)p == 0xffffffffffffffff)
	{
		ft_putstr("0xffffffffffffffff", fd);
		return (ft_lenstr("0xffffffffffffffff"));
	}
	ft_putstr("0x", fd);
	return (2 + custom_putnbr_base(fd, (unsigned long)p, 16));
}

int	print_d(int fd, va_list *ap)
{
	int	d;

	d = va_arg(*ap, int);
	return (custom_putnbr_base(fd, (int)d, 10));
}
