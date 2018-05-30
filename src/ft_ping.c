/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/05/30 09:26:53 by pribault         ###   ########.fr       */
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
	{0, NULL, 0},
};

void	connect_socket(t_env *env)
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	int	fd;

	if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		ft_error(2, ERROR_CANNOT_CREATE_SOCKET, NULL);
	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(env->address, NULL, &hints, &res))
		ft_error(2, ERROR_CANNOT_FIND_ADDRESS, env->address);
	socket_add_client_by_fd(env->socket, fd);
}

void	init_env(t_env *env)
{
	env->socket = socket_new();
	socket_attach_data(env->socket, env);
	socket_set_callback(env->socket, SOCKET_CLIENT_ADD_CB, &client_add);
	socket_set_callback(env->socket, SOCKET_CLIENT_DEL_CB, &client_add);
	socket_set_callback(env->socket, SOCKET_MSG_RECV_CB, msg_recv);
	socket_set_callback(env->socket, SOCKET_MSG_SEND_CB, msg_send);
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
	connect_socket(&env);
	while (1)
		socket_poll_events(env.socket, ALLOW_READ | ALLOW_WRITE);
	return (0);
}
