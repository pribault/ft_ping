/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/05/04 14:53:54 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_short_flag	g_short_flags[] = {
	{'\0', NULL}
};

t_long_flag		g_long_flags[] = {
	{NULL, 0, {0}, NULL}
};

t_error			g_errors[] = {
	{ERROR_CANNOT_CONNECT, "cannot connect to %s", 0},
	{0, NULL, 0},
};

void	init_env(t_env *env)
{
	env->socket = socket_new();
	socket_set_callback(env->socket, SOCKET_CLIENT_ADD_CB, &client_add);
	socket_set_callback(env->socket, SOCKET_CLIENT_DEL_CB, &client_add);
	socket_set_callback(env->socket, SOCKET_MSG_RECV_CB, msg_recv);
	socket_set_callback(env->socket, SOCKET_MSG_RECV_CB, msg_send);
}

int		main(int argc, char **argv)
{
	t_env	env;

	ft_add_errors((t_error *)&g_errors);
	init_env(&env);
	ft_get_flags(argc, argv, ft_get_flag_array((t_short_flag *)&g_short_flags,
		(t_long_flag *)&g_long_flags, (void *)&default_getter), &env);
	while (1)
		socket_poll_events(env.socket, ALLOW_READ | ALLOW_WRITE);
	return (0);
}
