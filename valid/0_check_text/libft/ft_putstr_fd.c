/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarcais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 10:13:03 by smarcais          #+#    #+#             */
/*   Updated: 2019/11/24 10:13:09 by smarcais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putstr_fd(char const *src, int fd)
{
	char	c;

	if (src)
	{
		while ((c = *src++) != '\0')
			write(fd, &c, 1);
	}
}