/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/06 00:01:51 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	icmp_time_exceeded(t_env *env, struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	size_t			idx;
	t_data			*data;
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((env->opt & OPT_VERBOSE) ? ft_error(2,
			ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
	gettimeofday(&now, NULL);
	idx = (size_t)-1;
	while (++idx < env->messages.n)
	{
		data = ft_vector_get(&env->messages, idx);
		if (size >= sizeof(struct iphdr) + sizeof(struct icmphdr) &&
			!ft_memcmp(data->msg.ptr + sizeof(struct iphdr),
				(void*)icmphdr + sizeof(struct iphdr) + sizeof(struct icmphdr),
				sizeof(struct icmphdr)))
		{
			ft_printf("From %s icmp_seq=%lu Time to live exceeded\n",
				inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)),
				data->seq);
			return (ft_vector_del_one(&env->messages, idx));
		}
	}
}
