/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_callback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 00:35:28 by pribault         ###   ########.fr       */
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
	t_env			*env;

	env = socket_get_data(socket);
	if (client_get_fd(client) > 2)
	{
		ft_printf("FT_PING %s (%s) %lu(%lu) bytes of data.\n",
			env->address, get_client_addr(client), env->packet_size,
			env->packet_size + sizeof(struct iphdr) + sizeof(struct icmphdr));
		ft_bzero(&hints, sizeof(struct addrinfo));
		hints.ai_socktype = ICMP;
		if (getaddrinfo(env->address, NULL, &hints, &result) != 0 ||
			!result || !result->ai_addr)
		{
			ft_error(2, ERROR_CANNOT_CONNECT, env->address);
			return (socket_remove_client(socket, client));
		}
		ft_memcpy(&client->addr, result->ai_addr, result->ai_addrlen);
		client->addr.len = result->ai_addrlen;
		client->write_type = WRITE_BY_ADDR;
		freeaddrinfo(result);
		env->client = client;
	}
}

void	client_del(t_socket *socket, t_client *client)
{
	t_env	*env;

	ft_printf("client removed\n");
	env = socket_get_data(socket);
	if (client == env->client)
		env->client = NULL;
}

void	client_excpt(t_socket *socket, t_client *client)
{
	(void)socket;
	(void)client;
	ft_printf("client exception\n");
}
