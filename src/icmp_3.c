/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/08 02:01:18 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 02:03:48 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

char	*get_icmp_redirect_string(__u8 code)
{
	static t_icmp_str	strs[] = {
		{ICMP_REDIR_NET, "Redirect Net"},
		{ICMP_REDIR_HOST, "Redirect Host"},
		{ICMP_REDIR_NETTOS, "Redirect Net for TOS"},
		{ICMP_REDIR_HOSTTOS, "Redirect Host for TOS"}
	};
	__u8				idx;

	idx = (__u8)-1;
	while (++idx < sizeof(strs) / sizeof(t_icmp_str))
		if (code == strs[idx].type)
			return (strs[idx].str);
	return ("Redirect");
}

void	icmp_redirect(struct iphdr *iphdr,
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
		get_icmp_redirect_string(icmphdr->code));
	g_e.lost++;
}
