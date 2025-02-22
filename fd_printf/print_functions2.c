/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 19:07:01 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/21 08:22:49 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fd_printf.h"

int	print_u(int fd, va_list *ap)
{
	int	u;

	u = va_arg(*ap, unsigned int);
	return (custom_putnbr_base(fd, (unsigned int)u, 10));
}

int	print_x(int fd, va_list *ap)
{
	int	x;

	x = va_arg(*ap, unsigned int);
	return (custom_putnbr_base(fd, (unsigned int)x, 16));
}

int	puthexa_big(int fd, unsigned int bx)
{
	char	*base;
	int		ret;

	ret = 1;
	base = "0123456789ABCDEF";
	if (bx / 16 != 0)
		ret = 1 + puthexa_big(fd, bx / 16);
	ft_putchar(base[bx % 16], fd);
	return (ret);
}

int	print_bx(int fd, va_list *ap)
{
	int	bx;

	bx = va_arg(*ap, unsigned int);
	return (puthexa_big(fd, bx));
}

int	print_prc(int fd, va_list *ap)
{
	(void)ap;
	ft_putchar('%', fd);
	return (1);
}
