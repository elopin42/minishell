/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 11:08:44 by tbeauman          #+#    #+#             */
/*   Updated: 2024/11/24 11:08:49 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	unsigned char	cc;

	if (c == -1)
		return (-1);
	cc = (unsigned char)c;
	if (cc >= 'a' && cc <= 'z')
		cc -= 32;
	return ((int)cc);
}
