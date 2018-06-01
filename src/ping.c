/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 21:04:29 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 00:37:22 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	fill_icmp_header(t_env *env, struct icmphdr *icmphdr)
{
	icmphdr->type = ICMP_ECHO;
	icmphdr->checksum = compute_sum(icmphdr, (sizeof(struct icmphdr) +
		env->packet_size) / 2);
	icmphdr->checksum = (icmphdr->checksum << 8) | (icmphdr->checksum >> 8);
	if (env->opt & OPT_VERBOSE)
		debug_icmp(icmphdr, sizeof(struct icmphdr) +
			env->packet_size);
}

void	fill_queue(void *queue, size_t size, struct timeval *now)
{
	size_t	i;
	uint8_t	c;

	c = 0x10;
	if (size >= sizeof(struct timeval))
	{
		i = sizeof(struct timeval);
		ft_memcpy(queue, now, sizeof(struct timeval));
	}
	else
		i = 0;
	while (i < size)
		*(uint8_t *)(queue + i++) = c++;
}

void	send_ping_request(t_env *env, t_client *client)
{
	t_data			data;

	gettimeofday(&data.timestamp, NULL);
	data.msg.size = sizeof(struct icmphdr) + env->packet_size;
	if (!(data.msg.ptr = malloc(data.msg.size)))
		return (ft_error(2, ERROR_ALLOCATION_2, NULL));
	ft_bzero(data.msg.ptr, data.msg.size);
	fill_queue(data.msg.ptr + sizeof(struct icmphdr), env->packet_size,
		&data.timestamp);
	fill_icmp_header(env, data.msg.ptr);
	data.seq = env->icmp_seq++;
	socket_enqueue_write(env->socket, client, &data.msg);
	ft_vector_add(&env->messages, &data);
}

void	manage_ping_requests(t_env *env)
{
	struct timeval	now;
	size_t			diff;

	gettimeofday(&now, NULL);
	diff = (now.tv_sec - env->prev.tv_sec) * 1000000 +
		(now.tv_usec - env->prev.tv_usec);
	if ((!env->prev.tv_sec && !env->prev.tv_usec) ||
		diff >= env->interval)
	{
		send_ping_request(env, env->client);
		env->prev = now;
		now.tv_sec = env->interval / 1000000;
		now.tv_usec = env->interval % 1000000;
	}
	else
	{
		now.tv_sec = (env->interval - diff) / 1000000;
		now.tv_usec = (env->interval - diff) % 1000000;
	}
	socket_set_timeout(env->socket, &now);
}
