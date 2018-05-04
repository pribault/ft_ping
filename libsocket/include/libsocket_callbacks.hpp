/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libsocket_callbacks.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 13:27:29 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:17:19 by pribault         ###   ########.fr       */
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

#ifndef LIBSOCKET_CALLBACKS_HPP
# define LIBSOCKET_CALLBACKS_HPP

# ifdef __cplusplus

#  include "libsocket_message.hpp"
#  include "libsocket_client.hpp"

namespace libsocket
{
	class	Socket;

	class	Callbacks
	{
		public:

			virtual void	client_add(Socket& socket, Client& client);
			virtual void	client_del(Socket&, Client&);
			virtual void	msg_recv(Socket&, Client&, Message&);
			virtual void	msg_send(Socket&, Client&, Message&);
			virtual void	msg_trash(Socket&, Client&, Message&);
			virtual void	client_excpt(Socket&, Client&);
			virtual void	socket_bind(Socket&);
			virtual void	socket_unbind(Socket&);
			virtual void	socket_excpt(Socket&);
			virtual void	buffer_full(Socket&);
	};
}

# else

#  error "c++ only header"

# endif

#endif
