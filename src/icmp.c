/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 09:22:36 by pribault         ###   ########.fr       */
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
		size));
	ft_memdump(icmphdr, size);
}

void	icmp_echo_reply(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			hostname[32];
	char			buffer[32];
	struct timeval	now;

	gettimeofday(&now, NULL);
	if (getnameinfo((void*)&g_e.client->addr.addr, g_e.client->addr.len,
		hostname, sizeof(hostname), NULL, 0, 0))
		hostname[0] = '\0';
	ft_printf("%lu bytes from %s (%s): ", size, &hostname,
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)));
	ft_printf("icmp_seq=%lu ttl=%hhu", icmphdr->un.echo.sequence,
		iphdr->ttl);
	if (size >= sizeof(struct icmphdr) + sizeof(struct timeval))
		ft_printf(" time=%.2f ms\n",
		((float)(now.tv_sec - ((struct timeval *)&icmphdr[1])->tv_sec) *
			1000) + ((float)(now.tv_usec -
				((struct timeval *)&icmphdr[1])->tv_usec) / 1000));
	else
		ft_printf("\n");
}

void	icmp_dest_unreach(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
			ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
	gettimeofday(&now, NULL);
	ft_printf("From %s icmp_seq=%lu Destination Net Unreachable\n",
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)),
		icmphdr->un.echo.sequence);
}

t_icmp_hdlr	g_hdlrs[] = {
	{ICMP_ECHOREPLY, &icmp_echo_reply},
	{ICMP_DEST_UNREACH, &icmp_dest_unreach},
	{ICMP_TIME_EXCEEDED, &icmp_time_exceeded}
};

void	treat_icmphdr(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	size_t	i;

	if (g_e.opt & OPT_VERBOSE)
		debug_icmp(icmphdr, size);
	if (compute_sum(icmphdr, size))
		return ((g_e.opt & OPT_VERBOSE) ?
			ft_error(2, ERROR_INVALID_CHECKSUM, NULL) : (void)0);
	i = (size_t)-1;
	while (++i < sizeof(g_hdlrs) / sizeof(t_icmp_hdlr))
		if (icmphdr->type == g_hdlrs[i].type)
			g_hdlrs[i].function(iphdr, icmphdr, size);
}
