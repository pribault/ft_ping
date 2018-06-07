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
	printf("icmphdr:\n");
	printf("\ttype: %u\n", icmphdr->type);
	printf("\tcode: %u\n", icmphdr->code);
	printf("\tchecksum: %hu\n", icmphdr->checksum);
	printf("\tsum found: %hu\n", compute_sum(icmphdr,
		size));
	ft_memdump(icmphdr, size);
}

void	update_stats(double ms)
{
	if (ms < g_e.min || !g_e.min)
		g_e.min = ms;
	if (ms > g_e.max || !g_e.max)
		g_e.max = ms;
	g_e.avg = ((g_e.avg * g_e.received) + ms) / (g_e.received + 1);
	g_e.sum += ms;
	g_e.sum2 += ms * ms;
	g_e.received++;
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
	printf("%lu bytes from %s (%s): ", size, (char *)&hostname,
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)));
	printf("icmp_seq=%hu ttl=%hhu", icmphdr->un.echo.sequence,
		iphdr->ttl);
	if (size >= sizeof(struct icmphdr) + sizeof(struct timeval))
		printf(" time=%.2f ms\n",
		((float)(now.tv_sec - ((struct timeval *)&icmphdr[1])->tv_sec) *
			1000) + ((float)(now.tv_usec -
				((struct timeval *)&icmphdr[1])->tv_usec) / 1000));
	else
		printf("\n");
	update_stats((double)(now.tv_sec - ((struct timeval *)&icmphdr[1])->tv_sec)
		* 1000 + (double)(now.tv_usec -
			((struct timeval *)&icmphdr[1])->tv_usec) / 1000);
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
	printf("From %s icmp_seq=%hu Destination Net Unreachable\n",
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)),
		((struct icmphdr *)((void*)&icmphdr[1] +
			sizeof(struct iphdr)))->un.echo.sequence);
	g_e.lost++;
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
	if (!g_e.start.tv_sec && !g_e.start.tv_usec)
		gettimeofday(&g_e.start, NULL);
	gettimeofday(&g_e.prev_recv, NULL);
	while (++i < sizeof(g_hdlrs) / sizeof(t_icmp_hdlr))
		if (icmphdr->type == g_hdlrs[i].type)
			g_hdlrs[i].function(iphdr, icmphdr, size);
}
