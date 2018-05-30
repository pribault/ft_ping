/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_callback.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/05/29 23:34:28 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	msg_recv(t_socket *socket, t_client *client, t_msg *msg)
{
	if (msg->size >= sizeof(struct iphdr))
		treat_iphdr(socket_get_data(socket), client, msg->ptr, msg->size);
	else
		ft_error(2, ERROR_PACKET_TOO_SMALL, (void *)msg->size);
}

void	msg_send(t_socket *socket, t_client *client, t_msg *msg)
{
	(void)socket;
	(void)client;
	(void)msg;
	ft_printf("message sended\n");
}
