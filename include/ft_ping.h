/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:13 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 01:23:31 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include "libsocket.h"
# include "ip.h"
# include "icmp.h"
# include <sys/time.h>
# include <arpa/inet.h>

# define OPT_VERBOSE	BYTE(0)

# define DEFAULT_PACKET_SIZE	56
# define DEFAULT_TTL			50
# define DEFAULT_INTERVAL		1000000

typedef enum		e_ping_errors
{
	ERROR_CANNOT_CONNECT = ERROR_FT_MAX,
	ERROR_CANNOT_CREATE_SOCKET,
	ERROR_PACKET_TOO_SMALL,
	ERROR_INVALID_CHECKSUM,
	ERROR_INVALID_IHL,
	ERROR_PROTOCOL_NOT_HANDLED,
	ERROR_ADDRESS_SET,
	ERROR_NO_ADDRESS,
	ERROR_CANNOT_FIND_ADDRESS,
	ERROR_ALLOCATION_2,
	ERROR_CANNOT_SET_OPTION,
	ERROR_MEMORY_CORRUPTED
}					t_ping_errors;

typedef enum		e_ipv4_protocol
{
	IPV4_PROTOCOL_ICMP = 1
}					t_ipv4_protocol;

typedef struct		s_data
{
	struct timeval	timestamp;
	t_msg			msg;
	size_t			seq;
}					t_data;

typedef struct		s_env
{
	t_socket		*socket;
	char			*address;
	t_client		*client;
	t_vector		messages;
	size_t			icmp_seq;
	size_t			interval;
	struct timeval	prev;
	size_t			packet_size;
	uint8_t			ttl;
	uint8_t			opt;
}					t_env;

typedef struct		s_icmp_hdlr
{
	uint8_t			type;
	void			(*function)(t_env *, struct iphdr *, struct icmphdr *,
					size_t);
}					t_icmp_hdlr;

void				default_getter(char *s, t_env *env);
void				get_verbose(t_env *env, char **args, int n);

void				client_add(t_socket *socket, t_client *client);
void				client_del(t_socket *socket, t_client *client);
void				client_excpt(t_socket *socket, t_client *client);
void				msg_recv(t_socket *socket, t_client *client, t_msg *msg);
void				msg_send(t_socket *socket, t_client *client, t_msg *msg);
void				msg_trash(t_socket *socket, t_client *client, t_msg *msg);

void				endian(void *ptr, size_t size);
void				endian_iphdr(struct iphdr *iphdr);

__sum16				compute_sum(void *ptr, size_t size);
void				treat_iphdr(t_env *env, struct iphdr *iphdr,
					size_t size);
void				treat_icmphdr(t_env *env, struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);
void				debug_iphdr(struct iphdr *iphdr);
void				debug_icmp(struct icmphdr *icmphdr, size_t size);

char				*get_client_addr(t_client *client);

void				manage_ping_requests(t_env *env);
void				send_ping_request(t_env *env, t_client *client);

#endif
