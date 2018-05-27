/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_callback.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/05/25 15:14:57 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

__sum16	compute_sum(void *ptr, size_t size)
{
	__sum16	sum;
	size_t		i;

	sum = 0;
	i = (size_t)-1;
	while (++i < size)
		sum += ((__sum16 *)ptr)[i];
	return (~sum);
}

void	msg_recv(t_socket *socket, t_client *client, t_msg *msg)
{
	struct iphdr	*iphdr;

	(void)socket;
	(void)client;
	ft_printf("message received:\nglobal size=%lu\n", msg->size);
	ft_putstr("\033[38;5;51m");
	ft_memdump(msg->ptr, msg->size);
	ft_putstr("\033[0m");
	iphdr = msg->ptr;
	ft_putstr("\033[38;5;213m");
	ft_memdump(iphdr, sizeof(struct iphdr));
	ft_putstr("\033[0m");
	// endian_iphdr(iphdr);
	ft_printf("sizeof(iphdr)=%lu\nsizeof(icmphdr)=%lu\n", sizeof(struct iphdr), sizeof(struct icmphdr));
	ft_printf("ihl: %u version: %u check: %hu\n", iphdr->ihl, iphdr->version, iphdr->check);
	ft_printf("sum founded: %hu\n", compute_sum(iphdr, iphdr->ihl * 2) + iphdr->check);
	ft_printf("sum founded: %hu\n", compute_sum(msg->ptr, msg->size) + iphdr->check);
	ft_printf("length: %hu\n", iphdr->tot_len);
	ft_printf("protocol: %hhu\n", iphdr->protocol);
	ft_printf("dest addr: ");
	ft_memdump(&iphdr->saddr, sizeof(iphdr->saddr));
	ft_printf("source addr: ");
	ft_memdump(&iphdr->daddr, sizeof(iphdr->daddr));
}

void	msg_send(t_socket *socket, t_client *client, t_msg *msg)
{
	(void)socket;
	(void)client;
	(void)msg;
	ft_printf("message sended\n");
}
