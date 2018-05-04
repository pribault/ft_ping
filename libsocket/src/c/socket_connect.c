/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_connect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 10:26:20 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:18:22 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Copyright © 2018 <pribault@student.42.fr>
**
**	Permission is hereby granted, free of charge, to any person obtaining a
**	copy of this software and associated documentation files (the “Software”),
**	to deal in the Software without restriction, including without limitation
**	the rights to use, copy, modify, merge, publish, distribute, sublicense,
**	and/or sell copies of the Software, and to permit persons to whom the
**	Software is furnished to do so, subject to the following conditions:
**
**	The above copyright notice and this permission notice shall be included in
**	all copies or substantial portions of the Software.
**
**	THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
**	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
**	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
**	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
**	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
**	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
**	OTHER DEALINGS IN THE SOFTWARE.
*/

#include "libsocket.h"

static int	iter_on_addresses(t_socket *socket, t_client *client,
			struct addrinfo *result)
{
	struct addrinfo	*addr;

	addr = result;
	while (addr)
	{
		if (connect(client->fd, addr->ai_addr, addr->ai_addrlen) >= 0)
		{
			ft_memcpy(&client->addr, addr->ai_addr, addr->ai_addrlen);
			client->addr.len = addr->ai_addrlen;
			client->write_type = WRITE_BY_ADDR;
			ft_vector_add(&socket->clients, client);
			if (socket->client_add)
				socket->client_add(socket, ft_vector_get(&socket->clients,
				socket->clients.n - 1));
			freeaddrinfo(result);
			return (1);
		}
		addr = addr->ai_next;
	}
	freeaddrinfo(result);
	return (0);
}

int			socket_connect(t_socket *msocket, t_method method, char *address,
			char *port)
{
	t_client		client;
	struct addrinfo	hints;
	struct addrinfo	*result;

	ft_bzero(&hints, sizeof(struct addrinfo));
	ft_bzero(&client, sizeof(t_client));
	hints.ai_family = method.domain;
	hints.ai_socktype = method.protocol;
	result = NULL;
	if (getaddrinfo(address, port, &hints, &result) != 0 ||
		(client.fd = socket(method.domain, method.protocol, 0)) < 0)
	{
		if (result)
			freeaddrinfo(result);
		return (0);
	}
	return (iter_on_addresses(msocket, &client, result));
}
