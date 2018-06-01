/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 01:32:52 by pribault         ###   ########.fr       */
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
	ft_memdump(&icmphdr[1], size - sizeof(struct icmphdr));
	write(1, &icmphdr[1], size - sizeof(struct icmphdr));
}

size_t	get_index(t_vector *messages, void *ptr, size_t size)
{
	t_data	*data;
	size_t	i;

	i = (size_t)-1;
	while (++i < messages->n)
	{
		data = ft_vector_get(messages, i);
		if (data->msg.size == size &&
			!ft_memcmp(data->msg.ptr + sizeof(struct icmphdr),
				ptr + sizeof(struct icmphdr),
				size - sizeof(struct icmphdr)))
			return (i);
	}
	return ((size_t)-1);
}

void	treat_icmphdr(t_env *env, struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	size_t			idx;
	t_data			*data;
	struct timeval	now;

	gettimeofday(&now, NULL);
	if (env->opt & OPT_VERBOSE)
		debug_icmp(icmphdr, size);
	if (compute_sum(icmphdr, size / 2))
		return (ft_error(2, ERROR_INVALID_CHECKSUM, NULL));
	if ((idx = get_index(&env->messages, icmphdr, size)) != (size_t)-1)
	{
		data = ft_vector_get(&env->messages, idx);
		ft_printf("%lu bytes from %s (%s): ", size,
			gethostbyaddr(&iphdr->saddr, 4, IPV4)->h_name,
			inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)));
		ft_printf("icmp_seq=%lu ttl=%hhu time=%.2f ms\n", data->seq,
			iphdr->ttl,
			((float)(now.tv_sec - data->timestamp.tv_sec) * 1000) +
			((float)(now.tv_usec - data->timestamp.tv_usec) / 1000));
		ft_vector_del_one(&env->messages, idx);
	}
}
