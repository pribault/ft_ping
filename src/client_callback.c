/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_callback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 12:59:06 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

char	*get_client_addr(t_client *client)
{
	static char	buffer[32];

	inet_ntop(AF_INET,
		&((struct sockaddr_in *)&client->addr.addr)->sin_addr,
		(char *)&buffer, sizeof(buffer));
	return ((char *)&buffer);
}

void	client_add(t_socket *socket, t_client *client)
{
	struct addrinfo	hints;
	struct addrinfo	*result;

	if (client_get_fd(client) > 2)
	{
		ft_bzero(&hints, sizeof(struct addrinfo));
		hints.ai_socktype = ICMP;
		if (getaddrinfo(g_e.address, NULL, &hints, &result) != 0 ||
			!result || !result->ai_addr)
		{
			ft_error(2, ERROR_CANNOT_CONNECT, g_e.address);
			return (socket_remove_client(socket, client));
		}
		ft_memcpy(&client->addr, result->ai_addr, result->ai_addrlen);
		client->addr.len = result->ai_addrlen;
		client->write_type = WRITE_BY_ADDR;
		freeaddrinfo(result);
		g_e.client = client;
		ft_printf("FT_PING %s (%s) %lu(%lu) bytes of data.\n",
			g_e.address, get_client_addr(client), g_e.packet_size,
			g_e.packet_size + sizeof(struct iphdr) + sizeof(struct icmphdr));
	}
}

void	client_del(t_socket *socket, t_client *client)
{
	(void)socket;
	if (client == g_e.client)
		g_e.client = NULL;
}

void	client_excpt(t_socket *socket, t_client *client)
{
	(void)socket;
	(void)client;
}
