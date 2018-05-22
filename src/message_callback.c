/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_callback.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/05/22 00:00:15 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	msg_recv(t_socket *socket, t_client *client, t_msg *msg)
{
	struct iphdr	*iphdr;

	(void)socket;
	(void)client;
	ft_printf("message received:\nglobal size=%lu\n", msg->size);
	ft_memdump(msg->ptr, msg->size);
	iphdr = msg->ptr;
	ft_printf("sizeof(iphdr)=%lu\nsizeof(icmphdr)=%lu\n", sizeof(struct iphdr), sizeof(struct icmphdr));
	ft_printf("ihl: %u version:%u\n", iphdr->ihl, iphdr->version);
}

void	msg_send(t_socket *socket, t_client *client, t_msg *msg)
{
	(void)socket;
	(void)client;
	(void)msg;
	ft_printf("message sended\n");
}
