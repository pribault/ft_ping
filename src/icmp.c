/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/05/30 21:59:19 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	debug_icmp(struct icmphdr *icmphdr, size_t size)
{
	ft_printf("icmphdr:\n");
	ft_printf("\ttype: %u\n", icmphdr->type);
	ft_printf("\tcode: %u\n", icmphdr->code);
	ft_printf("\tchecksum: %hu\n", icmphdr->checksum);
	ft_printf("\tsum found: %hu\n", compute_sum(icmphdr,
		size / 2));
}

void	treat_icmphdr(t_env *env, t_client *client,
		struct icmphdr *icmphdr, size_t size)
{
	(void)client;
	if (env->opt & OPT_VERBOSE)
		debug_icmp(icmphdr, size);
	if (compute_sum(icmphdr, size / 2))
	{
		ft_memdump(icmphdr, size);
		return (ft_error(2, ERROR_INVALID_CHECKSUM, NULL));
	}
	ft_printf("timestamp: %llx\n", time(NULL));
	ft_memdump(&icmphdr[1], size - sizeof(struct icmphdr));
	write(1, &icmphdr[1], size - sizeof(struct icmphdr));
	ft_putchar('\n');
}
