/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/06 00:05:17 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_short_flag	g_short_flags[] = {
	{'v', (void *)&get_verbose},
	{'\0', NULL}
};

t_long_flag		g_long_flags[] = {
	{"verbose", 0, {0}, (void *)&get_verbose},
	{"ttl", 1, {PARAM_UNSIGNED}, (void *)&get_ttl},
	{"interval", 1, {PARAM_FLOAT}, (void *)&get_interval},
	{"packetsize", 1, {PARAM_UNSIGNED}, (void *)&get_packet_size},
	{NULL, 0, {0}, NULL}
};

t_error			g_errors[] = {
	{ERROR_CANNOT_CONNECT, "cannot connect to %s", 0},
	{ERROR_CANNOT_CREATE_SOCKET, "cannot create socket", 0},
	{ERROR_PACKET_TOO_SMALL, "packet received too small, ignoring (%lu bytes)",
		0},
	{ERROR_INVALID_CHECKSUM, "invalid packet checksum", 0},
	{ERROR_INVALID_IHL, "invalid ihl", 0},
	{ERROR_INVALID_DEST_UNREACH, "invalid dest unreach icmp packet", 0},
	{ERROR_PROTOCOL_NOT_HANDLED, "protocol %u not handled", 0},
	{ERROR_ADDRESS_SET, "error already set to %s", 0},
	{ERROR_NO_ADDRESS, "no address set", ERROR_EXIT},
	{ERROR_CANNOT_FIND_ADDRESS, "cannot find address %s", ERROR_EXIT},
	{ERROR_ALLOCATION_2, "cannot allocate memory", 0},
	{ERROR_CANNOT_SET_OPTION, "cannot set socket options", 0},
	{ERROR_MEMORY_CORRUPTED, "memory corrupted", ERROR_EXIT},
	{ERROR_MINIMAL_INTERVAL,
		"cannot flood; mininal interval, allowed for user, is 200ms",
		ERROR_EXIT},
	{0, NULL, 0},
};

#include <errno.h>

int		create_icmp_socket(t_socket *sock)
{
	int				fd;
	int				n;

	n = 1;
	if ((fd = socket(IPV4, ICMP, IPPROTO_ICMP)) == -1 ||
		setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &n, sizeof(int)) == -1)
	{
		ft_error(2, ERROR_CANNOT_CREATE_SOCKET, NULL);
		return (0);
	}
	socket_add_client_by_fd(sock, fd);
	return (1);
}

void	buffer_full(t_socket *socket)
{
	t_env	*env;

	env = socket_get_data(socket);
	if (env->opt & OPT_VERBOSE)
		ft_printf("buffer full\n");
	socket_poll_events(socket, ALLOW_WRITE);
}

void	init_env(t_env *env)
{
	env->packet_size = DEFAULT_PACKET_SIZE;
	env->ttl = DEFAULT_TTL;
	env->interval = DEFAULT_INTERVAL;
	env->prev = (struct timeval){0, 0};
	env->socket = socket_new();
	env->icmp_seq = 1;
	ft_vector_init(&env->messages, ALLOC_MALLOC, sizeof(t_data));
	socket_attach_data(env->socket, env);
	socket_set_callback(env->socket, SOCKET_CLIENT_ADD_CB, &client_add);
	socket_set_callback(env->socket, SOCKET_CLIENT_DEL_CB, &client_add);
	socket_set_callback(env->socket, SOCKET_CLIENT_EXCEPTION_CB,
		&client_excpt);
	socket_set_callback(env->socket, SOCKET_MSG_RECV_CB, &msg_recv);
	socket_set_callback(env->socket, SOCKET_MSG_SEND_CB, &msg_send);
	socket_set_callback(env->socket, SOCKET_MSG_TRASH_CB, &msg_trash);
	socket_set_callback(env->socket, SOCKET_BUFFER_FULL_CB, &buffer_full);
}

int		main(int argc, char **argv)
{
	t_env	env;

	ft_bzero(&env, sizeof(t_env));
	ft_add_errors((t_error *)&g_errors);
	init_env(&env);
	ft_get_flags(argc, argv, ft_get_flag_array((t_short_flag *)&g_short_flags,
		(t_long_flag *)&g_long_flags, (void *)&default_getter), &env);
	if (!env.address)
		ft_error(2, ERROR_NO_ADDRESS, NULL);
	if (!create_icmp_socket(env.socket))
		ft_error(2, ERROR_CANNOT_CONNECT, env.address);
	while (1)
	{
		if (check_malloc() == MALLOC_CORRUPTED)
			ft_error(2, ERROR_MEMORY_CORRUPTED, NULL);
		if (env.client)
			manage_ping_requests(&env);
		socket_poll_events(env.socket, ALLOW_READ | ALLOW_WRITE);
	}
	return (0);
}
