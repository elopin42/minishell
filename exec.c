/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 03:06:31 by elopin            #+#    #+#             */
/*   Updated: 2025/02/21 03:24:10 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>

void	handler(int sig)
{
	exit (0);
}

// stp pour z et int pour c

int	main(void)
{
	while (42)
	{
		signal(SIGINT, handler);
		signal(SIGTSTP, handler);
	}
	return (0);
}
