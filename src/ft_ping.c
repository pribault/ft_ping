/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 12:59:37 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_env	g_e;

t_error	g_errors[] = {
	{ERROR_CANNOT_CONNECT, "cannot connect to %s", ERROR_EXIT},
	{ERROR_CANNOT_CREATE_SOCKET, "cannot create socket", 0},
	{ERROR_PACKET_TOO_SMALL,
		"packet received too small, ignoring (%lu bytes)", 0},
	{ERROR_INVALID_CHECKSUM, "invalid packet checksum", 0},
	{ERROR_INVALID_IHL, "invalid ihl", 0},
	{ERROR_INVALID_DEST_UNREACH, "invalid dest unreach icmp packet", 0},
	{ERROR_PROTOCOL_NOT_HANDLED, "protocol %u not handled", 0},
	{ERROR_ADDRESS_SET, "error already set to %s", 0},
	{ERROR_NO_ADDRESS, "no address set", 0},
	{ERROR_CANNOT_FIND_ADDRESS, "cannot find address %s", ERROR_EXIT},
	{ERROR_ALLOCATION_2, "cannot allocate memory", 0},
	{ERROR_CANNOT_SET_OPTION, "cannot set socket options", 0},
	{ERROR_MEMORY_CORRUPTED, "memory corrupted", ERROR_EXIT},
	{ERROR_MINIMAL_INTERVAL,
		"cannot flood; mininal interval, allowed for user, is 200ms",
		ERROR_EXIT},
	{0, NULL, 0},
};

/*
**	void	set_errors(void)
**	{
**		static t_error	errors[] = {
**			{ERROR_CANNOT_CONNECT, "cannot connect to %s", 0},
**			{ERROR_CANNOT_CREATE_SOCKET, "cannot create socket", 0},
**			{ERROR_PACKET_TOO_SMALL,
**			"packet received too small, ignoring (%lu bytes)", 0},
**			{ERROR_INVALID_CHECKSUM, "invalid packet checksum", 0},
**			{ERROR_INVALID_IHL, "invalid ihl", 0},
**			{ERROR_INVALID_DEST_UNREACH, "invalid dest unreach icmp packet", 0},
**			{ERROR_PROTOCOL_NOT_HANDLED, "protocol %u not handled", 0},
**			{ERROR_ADDRESS_SET, "error already set to %s", 0},
**			{ERROR_NO_ADDRESS, "no address set", ERROR_EXIT},
**			{ERROR_CANNOT_FIND_ADDRESS, "cannot find address %s", ERROR_EXIT},
**			{ERROR_ALLOCATION_2, "cannot allocate memory", 0},
**			{ERROR_CANNOT_SET_OPTION, "cannot set socket options", 0},
**			{ERROR_MEMORY_CORRUPTED, "memory corrupted", ERROR_EXIT},
**			{ERROR_MINIMAL_INTERVAL,
**				"cannot flood; mininal interval, allowed for user, is 200ms",
**				ERROR_EXIT},
**			{0, NULL, 0},
**		};
**
**		ft_add_errors((t_error *)&errors);
**	}
*/

void	set_errors(void)
{
	ft_add_errors((t_error *)&g_errors);
}

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
	socket_poll_events(socket, ALLOW_WRITE);
}

void	init_env(void)
{
	g_e.packet_size = DEFAULT_PACKET_SIZE;
	g_e.ttl = DEFAULT_TTL;
	g_e.interval = DEFAULT_INTERVAL;
	g_e.socket = socket_new();
	g_e.icmp_seq = 1;
	g_e.count = (size_t)-1;
	g_e.timeout = DEFAULT_TIMEOUT;
	g_e.min = 0;
	g_e.avg = 0;
	g_e.max = 0;
	g_e.sum = 0;
	g_e.sum2 = 0;
	socket_set_callback(g_e.socket, SOCKET_CLIENT_ADD_CB, &client_add);
	socket_set_callback(g_e.socket, SOCKET_CLIENT_DEL_CB, &client_del);
	socket_set_callback(g_e.socket, SOCKET_CLIENT_EXCEPTION_CB,
		&client_excpt);
	socket_set_callback(g_e.socket, SOCKET_MSG_RECV_CB, &msg_recv);
	socket_set_callback(g_e.socket, SOCKET_MSG_SEND_CB, &msg_send);
	socket_set_callback(g_e.socket, SOCKET_MSG_TRASH_CB, &msg_trash);
	socket_set_callback(g_e.socket, SOCKET_BUFFER_FULL_CB, &buffer_full);
}

int		main(int argc, char **argv)
{
	set_signals();
	ft_bzero(&g_e, sizeof(t_env));
	set_errors();
	init_env();
	get_flags(argc, argv);
	if (!g_e.address)
	{
		ft_error(2, ERROR_NO_ADDRESS, NULL);
		print_usage();
	}
	if (!create_icmp_socket(g_e.socket))
		ft_error(2, ERROR_CANNOT_CONNECT, g_e.address);
	while (1)
	{
		if (check_malloc() == MALLOC_CORRUPTED)
			ft_error(2, ERROR_MEMORY_CORRUPTED, NULL);
		if (g_e.client)
			manage_ping_requests();
		socket_poll_events(g_e.socket, ALLOW_READ | ALLOW_WRITE);
	}
	return (0);
}
