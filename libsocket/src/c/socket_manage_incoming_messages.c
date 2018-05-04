/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_manage_incoming_messages.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 11:05:18 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 14:19:34 by pribault         ###   ########.fr       */
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

static void	socket_get_client_message(t_socket *socket, t_client *client)
{
	char	buffer[socket->read_size];
	int		ret;
	t_msg	msg;

	if ((ret = read(client->fd, &buffer, socket->read_size)) > 0)
	{
		if (socket->msg_recv)
		{
			msg.ptr = &buffer;
			msg.size = ret;
			socket->msg_recv(socket, client, &msg);
		}
	}
	else if (!ret)
		socket_remove_client(socket, client);
}

void		socket_manage_incoming_messages(t_socket *socket, fd_set *set,
			fd_set *err_set, int *n_evts)
{
	t_vector	*vector;
	t_client	*client;
	size_t		i;

	if ((*n_evts) < 1)
		return ;
	vector = &socket->clients;
	i = vector->n;
	while (--i != (size_t)-1 && (*n_evts))
	{
		if ((client = ft_vector_get(vector, i)))
		{
			if (FD_ISSET(client->fd, err_set))
			{
				if (socket->client_excpt)
					socket->client_excpt(socket, client);
				(*n_evts)--;
			}
			else if (FD_ISSET(client->fd, set))
			{
				socket_get_client_message(socket, client);
				(*n_evts)--;
			}
		}
	}
}
