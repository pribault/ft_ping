/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 23:06:59 by pribault          #+#    #+#             */
/*   Updated: 2018/05/23 23:26:11 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#if defined (__LITTLE_ENDIAN_BITFIELD)

void	endian(void *ptr, size_t size)
{
	size_t	i;

	i = 0;
	while (i < --size)
		ft_swap(ptr + i++, ptr + size, 1);
}

#elif defined (__BIG_ENDIAN_BITFIELD)

void	endian(void *ptr, size_t size)
{
	(void)ptr;
	(void)size;
}

#endif

void	endian_iphdr(struct iphdr *iphdr)
{
	endian(&iphdr->tot_len, sizeof(iphdr->tot_len));
	endian(&iphdr->id, sizeof(iphdr->id));
	endian(&iphdr->frag_off, sizeof(iphdr->frag_off));
}
