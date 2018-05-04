/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_add_incoming_client.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 10:57:49 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 15:05:23 by pribault         ###   ########.fr       */
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

void	socket_add_incoming_client(t_socket *socket, int *n_evts)
{
	struct hostent	*host;
	t_client		client;

	ft_bzero(&client, sizeof(t_client));
	client.addr.len = sizeof(struct sockaddr);
	if ((*n_evts) < 1 ||
		(client.fd = accept(socket->sockfd, (void*)&client.addr,
		&client.addr.len)) < 0)
		return ;
	if ((host = gethostbyaddr(&client.addr.addr, client.addr.len,
		socket->domain)))
		client.addr.str = ft_strdup(host->h_name);
	client.write_type = WRITE_BY_ADDR;
	(*n_evts)--;
	ft_vector_add(&socket->clients, &client);
	if (socket->client_add)
		socket->client_add(socket, ft_vector_get(&socket->clients,
		socket->clients.n - 1));
}
