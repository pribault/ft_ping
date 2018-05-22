/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:13 by pribault          #+#    #+#             */
/*   Updated: 2018/05/21 23:44:10 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include "libsocket.h"
# include <linux/ip.h>
# include <linux/icmp.h>

typedef enum	e_ping_errors
{
	ERROR_CANNOT_CONNECT = ERROR_FT_MAX,
	ERROR_CANNOT_CREATE_SOCKET
}				t_ping_errors;

typedef struct	s_env
{
	t_socket	*socket;
}				t_env;

void			default_getter(char *s, t_env *env);

void			client_add(t_socket *socket, t_client *client);
void			client_del(t_socket *socket, t_client *client);
void			msg_recv(t_socket *socket, t_client *client, t_msg *msg);
void			msg_send(t_socket *socket, t_client *client, t_msg *msg);

#endif
