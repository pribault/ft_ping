/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_callback.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 00:56:31 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	msg_recv(t_socket *socket, t_client *client, t_msg *msg)
{
	(void)socket;
	(void)client;
	if (msg->size >= sizeof(struct iphdr))
	{
		treat_iphdr(msg->ptr, msg->size);
		if (g_e.count != (size_t)-1 &&
			g_e.lost + g_e.received >= g_e.count)
			print_statistics(0);
	}
	else
		ft_error(2, ERROR_PACKET_TOO_SMALL, (void *)msg->size);
}

void	msg_send(t_socket *socket, t_client *client, t_msg *msg)
{
	(void)socket;
	(void)client;
	(void)msg;
	g_e.transmitted++;
}

void	msg_trash(t_socket *socket, t_client *client, t_msg *msg)
{
	(void)socket;
	(void)client;
	(void)msg;
	free(msg->ptr);
}
