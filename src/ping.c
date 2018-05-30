/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 21:04:29 by pribault          #+#    #+#             */
/*   Updated: 2018/05/30 22:41:56 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	fill_ip_header(t_env *env, t_client *client, struct iphdr *iphdr)
{
	iphdr->ihl = 5;
	iphdr->version = 4;
	iphdr->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr) +
		env->packet_size;
	iphdr->ttl = env->ttl;
	iphdr->protocol = IPV4_PROTOCOL_ICMP;
	if (client->addr.len == sizeof(struct sockaddr_in))
	{
		ft_memcpy(&iphdr->daddr, (void *)&client->addr.addr + 4, 4);
		// iphdr->saddr = 0x0100007f;
		iphdr->saddr = 0x1000a8c0;
	}
	endian_iphdr(iphdr);
	iphdr->check = compute_sum(iphdr, 10);
	iphdr->check = (iphdr->check << 8) | (iphdr->check >> 8);
	debug_iphdr(iphdr);
}

void	fill_icmp_header(t_env *env, t_client *client,
		struct icmphdr *icmphdr)
{
	*((time_t *)&icmphdr[1]) = time(NULL);
	icmphdr->type = ICMP_ECHO;
	icmphdr->checksum = compute_sum(icmphdr, (sizeof(struct icmphdr) +
		env->packet_size) / 2);
	icmphdr->checksum = (icmphdr->checksum << 8) | (icmphdr->checksum >> 8);
	debug_icmp(icmphdr, sizeof(struct icmphdr) +
		env->packet_size);
}

void	send_ping_request(t_env *env, t_client *client)
{
	t_msg	msg;

	msg.size = sizeof(struct iphdr) + sizeof(struct icmphdr) +
		env->packet_size;
	if (!(msg.ptr = malloc(msg.size)))
		return (ft_error(2, ERROR_ALLOCATION_2, NULL));
	ft_bzero(msg.ptr, msg.size);
	fill_ip_header(env, client, msg.ptr);
	fill_icmp_header(env, client, msg.ptr + sizeof(struct iphdr));
	socket_enqueue_write(env->socket, client, &msg);
}
