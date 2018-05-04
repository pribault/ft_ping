/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_get_incoming_message.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 11:01:35 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:18:50 by pribault         ###   ########.fr       */
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

#define LIBSOCKET_INTERNAL

#include "libsocket.h"

static void	socket_add_client_udp(t_socket *socket, t_client *client)
{
	client->write_type = WRITE_BY_ADDR;
	ft_vector_add(&socket->clients, client);
	socket->client_add(socket, ft_vector_get(&socket->clients,
	socket->clients.n - 1));
}

void		socket_get_incoming_message(t_socket *socket, int *n_evts)
{
	char		buffer[socket->read_size];
	t_client	client;
	t_client	*new;
	t_msg		msg;
	int			ret;

	client.fd = socket->sockfd;
	client.addr.len = sizeof(struct sockaddr_storage);
	if ((ret = recvfrom(socket->sockfd, &buffer, socket->read_size,
		0, (void*)&client.addr.addr, &client.addr.len)) == -1)
		return ;
	(*n_evts)--;
	if (!(new = socket_find_client_by_address(socket, &client.addr.addr)) &&
		socket->client_add)
		socket_add_client_udp(socket, &client);
	if (socket->msg_recv)
	{
		msg = (t_msg){&buffer, ret};
		socket->msg_recv(socket, ft_vector_get(&socket->clients,
		socket->clients.n - 1), &msg);
	}
}
