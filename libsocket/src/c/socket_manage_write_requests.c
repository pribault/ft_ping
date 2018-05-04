/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_manage_write_requests.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 11:10:58 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:19:01 by pribault         ###   ########.fr       */
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

static int	socket_try_write(t_circ_buffer *buffer, t_towrite *towrite)
{
	if (towrite->client.write_type == WRITE_BY_FD)
	{
		if (write(towrite->client.fd, towrite->data.ptr,
			towrite->data.size) < 0)
		{
			ft_circ_buffer_enqueue(buffer, towrite);
			return (0);
		}
		return (1);
	}
	else if (towrite->client.write_type == WRITE_BY_ADDR)
	{
		if (sendto(towrite->client.fd, towrite->data.ptr,
			towrite->data.size, 0, (void*)&towrite->client.addr,
			towrite->client.addr.len) < 0)
		{
			ft_circ_buffer_enqueue(buffer, towrite);
			return (0);
		}
		return (1);
	}
	return (0);
}

void		socket_manage_write_requests(t_socket *socket, fd_set *set,
			int *n_evts)
{
	t_towrite		*towrite;
	t_circ_buffer	*buffer;
	uint64_t		size;
	uint64_t		i;

	if ((*n_evts) < 1)
		return ;
	buffer = &socket->write_queue;
	i = (uint64_t)-1;
	size = ft_circ_buffer_get_size(buffer);
	while (++i < size &&
		(towrite = ft_circ_buffer_dequeue(buffer)))
		if (FD_ISSET(towrite->client.fd, set))
		{
			FD_CLR(towrite->client.fd, set);
			if (!socket_try_write(buffer, towrite))
				return ;
			if (socket->msg_send)
				socket->msg_send(socket, &towrite->client, &towrite->data);
			(*n_evts)--;
		}
		else
			ft_circ_buffer_enqueue(buffer, towrite);
}
