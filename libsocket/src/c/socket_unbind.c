/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_unbind.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 14:33:36 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 14:18:31 by pribault         ###   ########.fr       */
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

void		socket_unbind(t_socket *socket)
{
	t_client	*client;
	t_towrite	*towrite;
	size_t		i;

	if (!(socket->opt & SERVER_RUNNING))
		return ;
	socket->opt &= ~SERVER_RUNNING;
	while ((towrite = ft_circ_buffer_dequeue(&socket->write_queue)))
		free(towrite->data.ptr);
	i = (size_t)-1;
	if (socket->protocol == TCP)
		while (++i < socket->clients.n)
			if ((client = ft_vector_get(&socket->clients, i)))
				close(client->fd);
	ft_vector_resize(&socket->clients, 0);
	close(socket->sockfd);
	if (socket->socket_unbind)
		socket->socket_unbind(socket);
}
