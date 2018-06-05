/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 08:54:03 by pribault         ###   ########.fr       */
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
}

size_t	get_index(t_vector *messages, void *ptr, size_t size)
{
	t_data	*data;
	size_t	i;

	i = (size_t)-1;
	while (++i < messages->n)
	{
		data = ft_vector_get(messages, i);
		if (data->msg.size - sizeof(struct iphdr) == size &&
			!ft_memcmp(data->msg.ptr + sizeof(struct iphdr) +
				sizeof(struct icmphdr), ptr + sizeof(struct icmphdr),
				size - sizeof(struct icmphdr)))
			return (i);
	}
	return ((size_t)-1);
}

void	icmp_echo_reply(t_env *env, struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			hostname[32];
	char			buffer[32];
	size_t			idx;
	t_data			*data;
	struct timeval	now;

	gettimeofday(&now, NULL);
	if ((idx = get_index(&env->messages, icmphdr, size)) != (size_t)-1)
	{
		data = ft_vector_get(&env->messages, idx);
		if (getnameinfo((void*)&env->client->addr.addr, env->client->addr.len,
			hostname, sizeof(hostname), NULL, 0, 0))
			hostname[0] = '\0';
		ft_printf("%lu bytes from %s (%s): ", size, &hostname,
			inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)));
		ft_printf("icmp_seq=%lu ttl=%hhu time=%.2f ms\n",
			data->seq, iphdr->ttl,
			((float)(now.tv_sec - data->timestamp.tv_sec) * 1000) +
			((float)(now.tv_usec - data->timestamp.tv_usec) / 1000));
		ft_vector_del_one(&env->messages, idx);
	}
}

void	icmp_dest_unreach(t_env *env, struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	(void)env;
	(void)iphdr;
	(void)icmphdr;
	(void)size;
	ft_printf("here\n");
}

t_icmp_hdlr	g_hdlrs[] = {
	{ICMP_ECHOREPLY, &icmp_echo_reply},
	{ICMP_DEST_UNREACH, &icmp_dest_unreach}
};

void	treat_icmphdr(t_env *env, struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	size_t	i;

	if (env->opt & OPT_VERBOSE)
		debug_icmp(icmphdr, size);
	if (compute_sum(icmphdr, size / 2))
		return (ft_error(2, ERROR_INVALID_CHECKSUM, NULL));
	i = (size_t)-1;
	while (++i < sizeof(g_hdlrs) / sizeof(t_icmp_hdlr))
		if (icmphdr->type == g_hdlrs[i].type)
			g_hdlrs[i].function(env, iphdr, icmphdr, size);
}
