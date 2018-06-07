/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 09:19:58 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	icmp_time_exceeded(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
			ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
	gettimeofday(&now, NULL);
	ft_printf("From %s icmp_seq=%lu Time to live exceeded\n",
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)),
		icmphdr->un.echo.sequence);
}