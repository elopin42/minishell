/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:43:45 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 16:45:58 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FD_PRINTF_H
# define FD_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
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
