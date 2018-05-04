/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libsocket.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/07 11:15:22 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:20:35 by pribault         ###   ########.fr       */
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

#ifndef LIBSOCKET_H
# define LIBSOCKET_H

# define LIBSOCKET_VERSION_MAJOR	2
# define LIBSOCKET_VERSION_MINOR	0
# define LIBSOCKET_VERSION_PATCH	0

# ifdef __cplusplus

#  include "libsocket_socket.hpp"

# else

/*
****************
**	includes  **
****************
*/

#  include <sys/socket.h>
#  include <inttypes.h>
#  include <unistd.h>
#  include <netdb.h>

#  include "libft.h"

#  include "libsocket_structures.h"

/*
************************
**	socket functions  **
************************
*/

/*
**	allocate a new socket, return null on error
*/

t_socket	*socket_new(void);

/*
**	delete a socket created with socket_new
*/

void		socket_delete(t_socket **socket);

/*
**	bind a socket to a port.
**	return 1 on succes, 0 on error.
**
**	errors leading to failure:
**	 - socket already bind
**	 - cannot create socket
**	 - cannot set options to socket
**	 - cannot bind socket
**	 - cannot listen on port (tcp only)
**
**	you can register a callback with socket_set_callback
**	to be notified of of bind and unbind events.
*/

int			socket_bind(t_socket *socket, t_method method, char *port);

/*
**	unbind a socket and clear all queues and buffers.
**	this function will silently fail if the socket is not binded.
**	you can register a callback with socket_set_callback
**	to be notified of of bind and unbind events.
*/

void		socket_unbind(t_socket *socket);

/*
**	set a callback to call when an event is generated.
**	available callbacks:
**	 - SOCKET_CLIENT_ADD_CB, will be called with (t_socket *, t_client *)
**	 - SOCKET_CLIENT_DEL_CB, will be called with (t_socket *, t_client *)
**	 - SOCKET_MSG_RECV_CB, will be called with (struct s_socket *, t_client *,
**		t_msg *)
**	 - SOCKET_MSG_SEND_CB, will be called with (struct s_socket *, t_client *,
**		t_msg *)
**	 - SOCKET_MSG_TRASH_CB, will be called with (struct s_socket *, t_client *,
**		t_msg *)
**	 - SOCKET_CLIENT_EXCEPTION_CB, will be called with (struct s_socket *,
**		t_client *)
**	 - SOCKET_BIND_CB, will be called with (struct s_socket *)
**	 - SOCKET_UNBIND_CB, will be called with (struct s_socket *)
**	 - SOCKET_EXCEPTION_CB, will be called with (struct s_socket *)
**	 - SOCKET_BUFFER_FULL_CB, will be called with (struct s_socket *)
*/

void		socket_set_callback(t_socket *socket, t_callback cb, void *ptr);

/*
**	try to connect to a distant server using address and port.
**	method is a libsocket structure containing the protocol used (tcp or udp),
**	and the domain (ipv4 or ipv6).
**
**	return 1 on succes, 0 on error.
**
**	errors leading to failure:
**	 - cannot create socket
**	 - cannot find address
*/

int			socket_connect(t_socket *msocket, t_method method, char *address,
			char *port);

/*
**	remove a client from a socket and close related connection.
*/

void		socket_remove_client(t_socket *socket, t_client *client);

/*
**	enqueue a message to a client
*/

void		socket_enqueue_write(t_socket *socket, t_client *client,
			t_msg *msg);

/*
**	enqueue a message to a file descriptor
*/

void		socket_enqueue_write_by_fd(t_socket *socket, int fd, t_msg *msg);

/*
**	enqueue a message to an address
*/

void		socket_enqueue_write_by_address(t_socket *socket,
			struct sockaddr_storage *address, t_msg *msg);

/*
**	add a client by its file descriptor.
*/

void		socket_add_client_by_fd(t_socket *socket, int fd);

/*
**	handle specified events with flags.
**	flags is a bitmask resulting of the binary 'or' operation of the
**	followings values:
**	 - ACCEPT_CONNECTIONS
**	 - ALLOW_READ
**	 - ALLOW_WRITE
*/

void		socket_poll_events(t_socket *socket, uint8_t flags);

/*
**	getter and setter for socket attached data
*/

void		socket_attach_data(t_socket *socket, void *data);
void		*socket_get_data(t_socket *socket);

/*
**	getter and setter for client attached data
*/

void		client_attach_data(t_client *client, void *data);
void		*client_get_data(t_client *client);

/*
**	get client address.
**	the returned structure contains the address, its len, and the hostname
**	string
*/

t_addr		*client_get_address(t_client *client);

/*
**	get the client's file descriptor
*/

int			client_get_fd(t_client *client);

/*
**	getter and setter for socket timeout.
**	this timeout is the time after which socker_poll_events will return
**	if no event is received (cannot read, cannot write, and no new client is
**	connecting)
*/

t_timeval	*socket_get_timeout(t_socket *socket);
void		socket_set_timeout(t_socket *socket, t_timeval *timeout);

/*
**	getter and setter for read buffer size.
**	this value is used to allocate the read buffer on the stack, so if too big
**	your program will segfault (stack overflow).
*/

void		socket_set_read_buffer_size(t_socket *socket, uint64_t read_size);
uint64_t	socket_get_read_buffer_size(t_socket *socket);

/*
**	following functions are used for internal purpose in libsocket
*/

#  ifdef LIBSOCKET_INTERNAL

/*
**	used to add an incoming client in socket_poll_events
*/

void		socket_add_incoming_client(t_socket *socket, int *n_evts);

/*
**	used to get an incoming message in socket_poll_events for udp protocol
*/

void		socket_get_incoming_message(t_socket *socket, int *n_evts);

/*
**	used to get incoming messages in socket_poll_events for tcp protocol
*/

void		socket_manage_incoming_messages(t_socket *socket, fd_set *set,
			fd_set *err_set, int *n_evts);

/*
**	used to manage enqueued messages for write in socket_poll_events
*/

void		socket_manage_write_requests(t_socket *socket, fd_set *set,
			int *n_evts);

/*
**	used to find a client by its address when using udp protocol
**	in socket_get_incoming_message
*/

t_client	*socket_find_client_by_address(t_socket *socket,
			struct sockaddr_storage *addr);

#  endif

# endif

#else

# pragma message __FILE__ "already included"

#endif
