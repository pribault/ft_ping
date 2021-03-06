/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libsocket_message.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 08:55:38 by pribault          #+#    #+#             */
/*   Updated: 2018/04/28 13:17:30 by pribault         ###   ########.fr       */
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

#ifndef LIBSOCKET_MESSAGE_HPP
# define LIBSOCKET_MESSAGE_HPP

# ifdef __cplusplus

#  include "libsocket_client.hpp"

#  include <inttypes.h>
#  include <string.h>
#  include <stdio.h>
#  include <string>
#  include <vector>

namespace	libsocket
{
	class	Message
	{
		public:

			//	constructors
			Message(void);
			Message(std::string& string);
			Message(void *ptr, uint64_t size);

			//	destructor
			~Message(void);

			//	setter
			void	set_destinary(Client &client);

			//	operators
			Message&	operator=(Message& message);
			Message&	operator=(std::string string);
			Message&	operator=(std::string& string);
			Message&	operator+=(Message& message);
			Message&	operator+=(std::string string);
			Message&	operator+=(std::string& string);

			Message&	append(Message& message);
			Message&	append(std::string string);
			Message&	append(std::string& string);
			Message&	append(void *ptr, uint64_t size);

			//	debug
			void	debug(void);
			void	print(void);

		protected:

			std::vector<uint8_t>	vector;

		private:

			Client	client;
			bool	null_terminated;
	};
}

# else

#  error "c++ only header"

# endif

#endif
