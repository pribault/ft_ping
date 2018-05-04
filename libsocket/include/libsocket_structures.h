/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libsocket_structures.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 13:34:52 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:17:37 by pribault         ###   ########.fr       */
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

#ifndef LIBSOCKET_STRUCTURES_H
# define LIBSOCKET_STRUCTURES_H

# ifndef __cplusplus

#  include "libsocket_enums.h"

#  include "libft.h"

typedef struct timeval		t_timeval;

typedef struct				s_addr
{
	struct sockaddr_storage	addr;
	socklen_t				len;
	char					*str;
}							t_addr;

typedef struct				s_method
{
	t_protocol				protocol;
	t_domain				domain;
}							t_method;

typedef struct				s_client
{
	int						fd;
	t_addr					addr;
	t_timeval				last;
	void					*data;
	t_write_type			write_type;
}							t_client;

typedef struct				s_msg
{
	void					*ptr;
	uint64_t				size;
}							t_msg;

typedef struct				s_towrite
{
	t_client				client;
	t_msg					data;
}							t_towrite;

typedef struct				s_socket
{
	t_protocol				protocol;
	t_domain				domain;
	int						sockfd;
	int						queue_max;
	t_timeval				timeout;
	uint64_t				read_size;
	t_vector				clients;
	t_circ_buffer			write_queue;
	uint16_t				port;
	uint8_t					opt;
	void					*data;
	void					(*client_add)(struct s_socket *, t_client *);
	void					(*client_del)(struct s_socket *, t_client *);
	void					(*msg_recv)(struct s_socket *, t_client *,
							t_msg *);
	void					(*msg_send)(struct s_socket *, t_client *,
							t_msg *);
	void					(*msg_trash)(struct s_socket *, t_client *,
							t_msg *);
	void					(*client_excpt)(struct s_socket *, t_client *);
	void					(*socket_bind)(struct s_socket *);
	void					(*socket_unbind)(struct s_socket *);
	void					(*socket_excpt)(struct s_socket *);
	void					(*buffer_full)(struct s_socket *);
}							t_socket;

# else

#  pragma message __FILE__ "C only header"

# endif

#else

# pragma message __FILE__ "already included"

#endif
