/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:13 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 09:11:58 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

/*
****************
**	includes  **
****************
*/

# include "libft.h"
# include "libsocket.h"
# include "ip.h"
# include "icmp.h"
# include <sys/time.h>
# include <arpa/inet.h>

/*
***************
**	defines  **
***************
*/

/*
**	options
*/

# define OPT_VERBOSE	BYTE(0)

/*
**	default values
*/

# define DEFAULT_PACKET_SIZE	56
# define DEFAULT_TTL			50
# define DEFAULT_INTERVAL		1000000

/*
*************
**	enums  **
*************
*/

typedef enum		e_ping_errors
{
	ERROR_CANNOT_CONNECT = ERROR_FT_MAX,
	ERROR_CANNOT_CREATE_SOCKET,
	ERROR_PACKET_TOO_SMALL,
	ERROR_INVALID_CHECKSUM,
	ERROR_INVALID_IHL,
	ERROR_INVALID_DEST_UNREACH,
	ERROR_PROTOCOL_NOT_HANDLED,
	ERROR_ADDRESS_SET,
	ERROR_NO_ADDRESS,
	ERROR_CANNOT_FIND_ADDRESS,
	ERROR_ALLOCATION_2,
	ERROR_CANNOT_SET_OPTION,
	ERROR_MEMORY_CORRUPTED,
	ERROR_MINIMAL_INTERVAL
}					t_ping_errors;

typedef enum		e_ipv4_protocol
{
	IPV4_PROTOCOL_ICMP = 1
}					t_ipv4_protocol;

/*
******************
**	structures  **
******************
*/

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
	void			(*function)(struct iphdr *, struct icmphdr *,
					size_t);
}					t_icmp_hdlr;

/*
******************
**	prototypes  **
******************
*/

/*
**	flag getters
*/

void				default_getter(char *s, t_env *env);
void				get_verbose(t_env *env, char **args, int n);
void				get_ttl(t_env *env, char **args, int n);
void				get_interval(t_env *env, char **args, int n);
void				get_packet_size(t_env *env, char **args, int n);

/*
**	libsocket callbacks
*/

void				client_add(t_socket *socket, t_client *client);
void				client_del(t_socket *socket, t_client *client);
void				client_excpt(t_socket *socket, t_client *client);
void				msg_recv(t_socket *socket, t_client *client, t_msg *msg);
void				msg_send(t_socket *socket, t_client *client, t_msg *msg);
void				msg_trash(t_socket *socket, t_client *client, t_msg *msg);

/*
**	endian functions
*/

void				endian(void *ptr, size_t size);
void				endian_iphdr(struct iphdr *iphdr);

/*
**	packet treatment
*/

__sum16				compute_sum(void *ptr, size_t size);
void				treat_iphdr(struct iphdr *iphdr,
					size_t size);
void				treat_icmphdr(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);
void				debug_iphdr(struct iphdr *iphdr);
void				debug_icmp(struct icmphdr *icmphdr, size_t size);

/*
**	ping functions
*/

void				manage_ping_requests(void);
void				send_ping_request(t_client *client);

/*
**	icmp functions
*/

void				icmp_echo_reply(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);
void				icmp_dest_unreach(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);
void				icmp_time_exceeded(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);

/*
**	others
*/

void				print_statistics(int signal);
char				*get_client_addr(t_client *client);

extern t_env	g_e;

#endif
