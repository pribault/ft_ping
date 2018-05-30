/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 21:17:12 by pribault          #+#    #+#             */
/*   Updated: 2018/05/30 09:14:39 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

__sum16	compute_sum(void *ptr, size_t size)
{
	uint32_t	sum;
	size_t		i;

	sum = 0;
	i = (size_t)-1;
	while (++i < size)
		sum += (((((__sum16 *)ptr)[i] & 0xff00) >> 8) +
			((((__sum16 *)ptr)[i] & 0xff) << 8));
	return (~((sum & 0xffff) + ((sum & 0xffff0000) >> 16)));
}

void	debug_iphdr(struct iphdr *iphdr)
{
	ft_printf("iphdr:\n");
	ft_printf("\tihl: %u\n", iphdr->ihl);
	ft_printf("\tversion: %u\n", iphdr->version);
	ft_printf("\tcheck: %hu\n", iphdr->check);
	ft_printf("\tlength: %hu\n", iphdr->tot_len);
	ft_printf("\tprotocol: %hhu\n", iphdr->protocol);
	ft_printf("\tsource addr: %hhu.%hhu.%hhu.%hhu\n",
		((uint8_t *)&iphdr->saddr)[0], ((uint8_t *)&iphdr->saddr)[1],
		((uint8_t *)&iphdr->saddr)[2], ((uint8_t *)&iphdr->saddr)[3]);
	ft_printf("\tdest addr: %hhu.%hhu.%hhu.%hhu\n",
		((uint8_t *)&iphdr->daddr)[0], ((uint8_t *)&iphdr->daddr)[1],
		((uint8_t *)&iphdr->daddr)[2], ((uint8_t *)&iphdr->daddr)[3]);
	ft_printf("\tsum found: %hu\n", compute_sum(iphdr, iphdr->ihl * 2));
}

void	treat_iphdr(t_env *env, t_client *client,
		struct iphdr *iphdr, size_t size)
{
	// endian_iphdr(iphdr);
	if (iphdr->ihl * 4 > size)
		return (ft_error(2, ERROR_INVALID_IHL, 0));
	if (env->opt & OPT_VERBOSE)
		debug_iphdr(iphdr);
	if (compute_sum(iphdr, iphdr->ihl * 2))
	{
		ft_memdump(iphdr, sizeof(struct iphdr));
		return (ft_error(2, ERROR_INVALID_CHECKSUM, 0));
	}
	if (iphdr->protocol == IPV4_PROTOCOL_ICMP)
	{
		if (iphdr->ihl * 4 + sizeof(struct icmphdr) > size)
			return (ft_error(2, ERROR_PACKET_TOO_SMALL, (void *)size));
		else
			treat_icmphdr(env, client, (void *)iphdr + iphdr->ihl * 4,
				size - iphdr->ihl * 4);
	}
	else
		return (ft_error(2, ERROR_PROTOCOL_NOT_HANDLED,
			(void *)(size_t)iphdr->protocol));
}
