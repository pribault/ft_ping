/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:13 by pribault          #+#    #+#             */
/*   Updated: 2018/05/30 09:18:16 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include "libsocket.h"
# include <linux/ip.h>
# include <linux/icmp.h>
# include <time.h>

# define OPT_VERBOSE	BYTE(0)

typedef enum	e_ping_errors
{
	ERROR_CANNOT_CONNECT = ERROR_FT_MAX,
	ERROR_CANNOT_CREATE_SOCKET,
	ERROR_PACKET_TOO_SMALL,
	ERROR_INVALID_CHECKSUM,
	ERROR_INVALID_IHL,
	ERROR_PROTOCOL_NOT_HANDLED,
	ERROR_ADDRESS_SET,
	ERROR_NO_ADDRESS,
	ERROR_CANNOT_FIND_ADDRESS
}				t_ping_errors;

typedef enum	e_ipv4_protocol
{
	IPV4_PROTOCOL_ICMP = 1
}				t_ipv4_protocol;

typedef struct	s_env
{
	char		*address;
	t_socket	*socket;
	uint8_t		opt;
}				t_env;

void			default_getter(char *s, t_env *env);
void			get_verbose(t_env *env, char **args, int n);

void			client_add(t_socket *socket, t_client *client);
void			client_del(t_socket *socket, t_client *client);
void			msg_recv(t_socket *socket, t_client *client, t_msg *msg);
void			msg_send(t_socket *socket, t_client *client, t_msg *msg);

void			endian(void *ptr, size_t size);
void			endian_iphdr(struct iphdr *iphdr);

__sum16			compute_sum(void *ptr, size_t size);
void			treat_iphdr(t_env *env, t_client *client,
				struct iphdr *iphdr, size_t size);
void			treat_icmphdr(t_env *env, t_client *client,
				struct icmphdr *icmphdr, size_t size);
void			debug_iphdr(struct iphdr *iphdr);

#endif
