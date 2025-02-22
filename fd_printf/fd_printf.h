/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:56:08 by tbeauman          #+#    #+#             */
/*   Updated: 2025/02/21 08:09:46 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>

void	ft_putchar(char c, int fd);
size_t	ft_lenstr(const char *str);
void	ft_putstr(char *s, int fd);
int		print_c(int fd, va_list *ap);
int		print_s(int fd, va_list *ap);
int		custom_putnbr_base(int fd, long p, int b);
int		print_p(int fd, va_list *ap);
int		print_d(int fd, va_list *ap);
int		print_u(int fd, va_list *ap);
int		print_x(int fd, va_list *ap);
int		puthexa_big(int fd, unsigned int bx);
int		print_bx(int fd, va_list *ap);
int		print_prc(int fd, va_list *ap);

int		fd_printf(int fd, const char *frm, ...);

#endif
