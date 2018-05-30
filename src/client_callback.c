/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_callback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/05/30 22:55:36 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	client_add(t_socket *socket, t_client *client)
{
	int	optval;

	ft_printf("client added\n");
	if (setsockopt(client_get_fd(client), IPPROTO_IP, IP_HDRINCL, &optval,
		sizeof(int)) == -1)
	{
		ft_error(2, ERROR_CANNOT_SET_OPTION, NULL);
		socket_remove_client(socket, client);
	}
	else if (client_get_fd(client) > 2)
		send_ping_request(socket_get_data(socket), client);
}

void	client_del(t_socket *socket, t_client *client)
{
	(void)socket;
	(void)client;
	ft_printf("client removed\n");
}

void	client_excpt(t_socket *socket, t_client *client)
{
	(void)socket;
	(void)client;
	ft_printf("client exception\n");
}
