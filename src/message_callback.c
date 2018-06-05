/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_callback.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 00:17:10 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	msg_recv(t_socket *socket, t_client *client, t_msg *msg)
{
	t_env	*env;

	(void)client;
	env = socket_get_data(socket);
	if (env->opt & OPT_VERBOSE)
		ft_printf("message received\n");
	if (msg->size >= sizeof(struct iphdr))
		treat_iphdr(socket_get_data(socket), msg->ptr, msg->size);
	else
		ft_error(2, ERROR_PACKET_TOO_SMALL, (void *)msg->size);
}

void	msg_send(t_socket *socket, t_client *client, t_msg *msg)
{
	t_env	*env;

	(void)client;
	(void)msg;
	env = socket_get_data(socket);
	if (env->opt & OPT_VERBOSE)
		ft_printf("message sended\n");
}

void	msg_trash(t_socket *socket, t_client *client, t_msg *msg)
{
	t_env	*env;

	(void)client;
	(void)msg;
	env = socket_get_data(socket);
	if (env->opt & OPT_VERBOSE)
		ft_printf("message trashed\n");
	free(msg->ptr);
}
