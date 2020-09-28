/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarcais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 10:03:18 by smarcais          #+#    #+#             */
/*   Updated: 2019/11/28 17:08:48 by smarcais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putstr(const char *src)
{
	char	c;

	if (src)
	{
		while ((c = *src++) != '\0')
			write(1, &c, 1);
	}
}
