/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 21:17:12 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 00:37:51 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

__sum16	compute_sum(void *ptr, size_t size)
{
	uint32_t	sum;
	size_t		words;
	size_t		i;

	sum = 0;
	i = (size_t)-1;
	words = size / 2;
	while (++i < words)
		sum += (((((__sum16 *)ptr)[i] & 0xff00) >> 8) +
			((((__sum16 *)ptr)[i] & 0xff) << 8));
	if (size % 2)
		sum += *(uint8_t *)(ptr + size - 1) << 8;
	return (~((sum & 0xffff) + ((sum & 0xffff0000) >> 16)));
}

void	debug_iphdr(struct iphdr *iphdr)
{
	printf("iphdr:\n");
	printf("\tihl: %u\n", iphdr->ihl);
	printf("\tversion: %u\n", iphdr->version);
	printf("\tcheck: %hu\n", iphdr->check);
	printf("\ttos: %hhu\n", iphdr->tos);
	printf("\tlength: %hu\n", iphdr->tot_len);
	printf("\tid: %hu\n", iphdr->id);
	printf("\tfragment offset: %hu\n", iphdr->frag_off);
	printf("\tttl: %hhu\n", iphdr->ttl);
	printf("\tprotocol: %hhu\n", iphdr->protocol);
	printf("\tsource addr: %hhu.%hhu.%hhu.%hhu\n",
		((uint8_t *)&iphdr->saddr)[0], ((uint8_t *)&iphdr->saddr)[1],
		((uint8_t *)&iphdr->saddr)[2], ((uint8_t *)&iphdr->saddr)[3]);
	printf("\tdest addr: %hhu.%hhu.%hhu.%hhu\n",
		((uint8_t *)&iphdr->daddr)[0], ((uint8_t *)&iphdr->daddr)[1],
		((uint8_t *)&iphdr->daddr)[2], ((uint8_t *)&iphdr->daddr)[3]);
	printf("\tsum found: %hu\n", compute_sum(iphdr, iphdr->ihl * 4));
}

/*
**	this is badly indented, but forced to because of norminette <3
*/

void	treat_iphdr(struct iphdr *iphdr, size_t size)
{
	if (iphdr->ihl * 4 > size)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2, ERROR_INVALID_IHL, 0) :
			(void)0);
	if (compute_sum(iphdr, iphdr->ihl * 4))
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2, ERROR_INVALID_CHECKSUM,
		0) : (void)0);
		if (iphdr->protocol == IPV4_PROTOCOL_ICMP)
	{
		if (iphdr->ihl * 4 + sizeof(struct icmphdr) > size)
			return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
				ERROR_PACKET_TOO_SMALL, (void *)size) : (void)0);
		else
			treat_icmphdr(iphdr, (void *)iphdr + iphdr->ihl * 4,
				size - iphdr->ihl * 4);
	}
	else
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
			ERROR_PROTOCOL_NOT_HANDLED, (void *)(size_t)iphdr->protocol) :
				(void)0);
}
