/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_remove_client.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 11:20:23 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:19:10 by pribault         ###   ########.fr       */
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

static void	socket_remove_client_by_fd(t_vector *vector,
			t_client *client)
{
	t_client	*tmp;
	size_t		i;

	i = vector->n;
	while (--i != (size_t)-1)
	{
		if ((tmp = ft_vector_get(vector, i)) &&
			tmp->fd == client->fd)
		{
			ft_vector_del_one(vector, i);
			return ;
		}
	}
}

void		socket_remove_client(t_socket *socket, t_client *client)
{
	t_vector	*vector;

	vector = &socket->clients;
	if (socket->client_del)
		socket->client_del(socket, client);
	close(client->fd);
	if ((void*)client >= vector->ptr &&
		(void*)client < vector->ptr + vector->size)
		ft_vector_del_one(vector, ((void*)client - vector->ptr) /
		vector->type);
	else
		socket_remove_client_by_fd(vector, client);
}
