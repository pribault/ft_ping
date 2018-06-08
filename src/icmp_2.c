/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 09:58:12 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

char	*get_icmp_dest_unreach_string(__u8 code)
{
	static t_icmp_str	strs[] = {
		{ICMP_NET_UNREACH, "Destination Net Unreachable"},
		{ICMP_HOST_UNREACH, "Destination Host Unreachable"},
		{ICMP_PROT_UNREACH, "Destination Prot Unreachable"},
		{ICMP_PORT_UNREACH, "Destination Port Unreachable"},
		{ICMP_FRAG_NEEDED, "Fragmentation Needed/DF set"},
		{ICMP_SR_FAILED, "Source Route failed"},
		{ICMP_NET_UNKNOWN, "Net Unknown"},
		{ICMP_HOST_UNKNOWN, "Host Unknown"},
		{ICMP_HOST_ISOLATED, "Host Isolated"},
		{ICMP_NET_ANO, "Net ANO"},
		{ICMP_HOST_ANO, "Host ANO"},
		{ICMP_NET_UNR_TOS, "Net UNR Tos"},
		{ICMP_HOST_UNR_TOS, "Host UNR Tos"},
		{ICMP_PKT_FILTERED, "Packet filtered"},
		{ICMP_PREC_VIOLATION, "Precedence violation"},
		{ICMP_PREC_CUTOFF, "Precedence cut off"}
	};
	__u8				idx;

	idx = (__u8)-1;
	while (++idx < sizeof(strs) / sizeof(t_icmp_str))
		if (code == strs[idx].type)
			return (strs[idx].str);
	return ("Destination Unreachable");
}

void	icmp_dest_unreach(struct iphdr *iphdr,
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
		get_icmp_dest_unreach_string(icmphdr->code));
	g_e.lost++;
}

char	*get_icmp_time_exceeded_string(__u8 code)
{
	static t_icmp_str	strs[] = {
		{ICMP_EXC_TTL, "Time to live exceeded"},
		{ICMP_EXC_FRAGTIME, "Fragment Reassembly time exceeded"}
	};
	__u8				idx;

	idx = (__u8)-1;
	while (++idx < sizeof(strs) / sizeof(t_icmp_str))
		if (code == strs[idx].type)
			return (strs[idx].str);
	return ("Time Exceeded");
}

void	icmp_time_exceeded(struct iphdr *iphdr,
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
		get_icmp_time_exceeded_string(icmphdr->code));
	g_e.lost++;
}
