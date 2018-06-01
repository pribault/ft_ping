/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 00:36:36 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_short_flag	g_short_flags[] = {
	{'v', (void *)&get_verbose},
	{'\0', NULL}
};

t_long_flag		g_long_flags[] = {
	{"verbose", 0, {0}, (void *)&get_verbose},
	{NULL, 0, {0}, NULL}
};

t_error			g_errors[] = {
	{ERROR_CANNOT_CONNECT, "cannot connect to %s", 0},
	{ERROR_CANNOT_CREATE_SOCKET, "cannot create socket", 0},
	{ERROR_PACKET_TOO_SMALL, "packet received too small, ignoring (%lu bytes)",
		0},
	{ERROR_INVALID_CHECKSUM, "invalid packet checksum", 0},
	{ERROR_INVALID_IHL, "invalid ihl", 0},
	{ERROR_PROTOCOL_NOT_HANDLED, "protocol %u not handled", 0},
	{ERROR_ADDRESS_SET, "error already set to %s", 0},
	{ERROR_NO_ADDRESS, "no address set", ERROR_EXIT},
	{ERROR_CANNOT_FIND_ADDRESS, "cannot find address %s", ERROR_EXIT},
	{ERROR_ALLOCATION_2, "cannot allocate memory", 0},
	{ERROR_CANNOT_SET_OPTION, "cannot set socket options", 0},
	{ERROR_MEMORY_CORRUPTED, "memory corrupted", ERROR_EXIT},
	{0, NULL, 0},
};

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
	if (!socket_connect(env.socket, (t_method){ICMP, IPV4}, env.address, NULL))
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
