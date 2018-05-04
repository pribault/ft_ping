/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 11:32:44 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:19:04 by pribault         ###   ########.fr       */
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

static void	socket_trash_callback(t_socket *socket, t_towrite *towrite)
{
	if (socket->msg_trash)
		socket->msg_trash(socket, &towrite->client, &towrite->data);
}

t_socket	*socket_new(void)
{
	t_socket	*socket;

	if (!(socket = (t_socket*)malloc(sizeof(t_socket))))
		return (NULL);
	ft_bzero(socket, sizeof(t_socket));
	ft_vector_init(&socket->clients, ALLOC_MALLOC, sizeof(t_client));
	ft_circ_buffer_init(&socket->write_queue, ALLOC_MALLOC,
	sizeof(t_towrite), CIRCULAR_BUFFER_SIZE);
	ft_circ_buffer_set_trash_callback(&socket->write_queue,
	(void*)&socket_trash_callback, socket);
	socket->queue_max = DEFAULT_QUEUE_MAX;
	socket->timeout = TIMEVAL(DEFAULT_TIMEOUT_S, DEFAULT_TIMEOUT_US);
	socket->read_size = DEFAULT_READ_BUFFER_SIZE;
	socket->opt = 0;
	return (socket);
}
