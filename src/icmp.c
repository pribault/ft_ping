/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 01:57:22 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

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

char	*get_icmp_string(__u8 type)
{
	static t_icmp_str	strs[] = {
		{ICMP_ECHOREPLY, "Echo Reply"},
		{ICMP_DEST_UNREACH, "Desination Unreachable"},
		{ICMP_SOURCE_QUENCH, "Source Quench"},
		{ICMP_REDIRECT, "Redirect (change route)"},
		{ICMP_ECHO, "Echo"},
		{ICMP_TIME_EXCEEDED, "Time exceeded"},
		{ICMP_PARAMETERPROB, "Parameter Problem"},
		{ICMP_TIMESTAMP, "Timestamp Request"},
		{ICMP_TIMESTAMPREPLY, "Timestamp Reply"},
		{ICMP_INFO_REQUEST, "Information Request"},
		{ICMP_INFO_REPLY, "Information Reply"},
		{ICMP_ADDRESS, "Address Mask Request"},
		{ICMP_ADDRESSREPLY, "Address Mask Reply"}
	};
	__u8				idx;

	idx = (__u8)-1;
	while (++idx < sizeof(strs) / sizeof(t_icmp_str))
		if (type == strs[idx].type)
			return (strs[idx].str);
	return ("Unknown");
}

void	icmp_other(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
		ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
	if (((struct icmphdr *)((void*)&icmphdr[1] +
		sizeof(struct iphdr)))->un.echo.id != getpid())
		return ;
	gettimeofday(&now, NULL);
	printf("From %s icmp_seq=%hu %s\n",
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)),
		((struct icmphdr *)((void*)&icmphdr[1] +
			sizeof(struct iphdr)))->un.echo.sequence,
		get_icmp_string(icmphdr->type));
	g_e.lost++;
}

void	icmp_echo_reply(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			hostname[32];
	char			buffer[32];
	struct timeval	now;

	if (icmphdr->un.echo.id != getpid())
		return ;
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

void	treat_icmphdr(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	static t_icmp_hdlr	hdlrs[] = {
		{ICMP_ECHOREPLY, &icmp_echo_reply},
		{ICMP_DEST_UNREACH, &icmp_dest_unreach},
		{ICMP_TIME_EXCEEDED, &icmp_time_exceeded}
	};
	size_t	i;

	if (compute_sum(icmphdr, size))
		return ((g_e.opt & OPT_VERBOSE) ?
			ft_error(2, ERROR_INVALID_CHECKSUM, NULL) : (void)0);
		i = (size_t)-1;
	if (!g_e.start.tv_sec && !g_e.start.tv_usec)
		gettimeofday(&g_e.start, NULL);
	gettimeofday(&g_e.prev_recv, NULL);
	while (++i < sizeof(hdlrs) / sizeof(t_icmp_hdlr))
		if (icmphdr->type == hdlrs[i].type)
			return (hdlrs[i].function(iphdr, icmphdr, size));
	icmp_other(iphdr, icmphdr, size);
}
