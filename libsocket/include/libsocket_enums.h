/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libsocket_enums.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 13:35:56 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:17:27 by pribault         ###   ########.fr       */
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

#ifndef LIBSOCKET_ENUMS_H
# define LIBSOCKET_ENUMS_H

# ifndef __cplusplus

#  include "libsocket_defines.h"

typedef enum	e_protocol
{
	TCP = SOCK_STREAM,
	UDP = SOCK_DGRAM
}				t_protocol;

typedef enum	e_domain
{
	IPV4 = AF_INET,
	IPV6 = AF_INET6
}				t_domain;

typedef enum	e_write_type
{
	WRITE_BY_FD,
	WRITE_BY_ADDR
}				t_write_type;

/*
**	callbacks used with socket_set_callback
*/

typedef enum	e_callback
{
	SOCKET_CLIENT_ADD_CB = 0,
	SOCKET_CLIENT_DEL_CB,
	SOCKET_MSG_RECV_CB,
	SOCKET_MSG_SEND_CB,
	SOCKET_MSG_TRASH_CB,
	SOCKET_CLIENT_EXCEPTION_CB,
	SOCKET_BIND_CB,
	SOCKET_UNBIND_CB,
	SOCKET_EXCEPTION_CB,
	SOCKET_BUFFER_FULL_CB,
	SOCKET_CALLBACK_MAX
}				t_callback;

# else

#  pragma message __FILE__ "C only header"

# endif

#else

# pragma message __FILE__ "already included"

#endif
