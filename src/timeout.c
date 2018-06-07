/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeout.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 23:54:57 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 23:46:27 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	set_timeout(struct timeval *timeout, struct timeval *now)
{
	size_t	diff_send;
	size_t	diff_recv;

	diff_send = (now->tv_sec - g_e.prev_send.tv_sec) * 1000000 +
		(now->tv_usec - g_e.prev_send.tv_usec);
	diff_recv = (now->tv_sec - g_e.prev_recv.tv_sec) * 1000000 +
		(now->tv_usec - g_e.prev_recv.tv_usec);
	if (((g_e.prev_send.tv_sec || g_e.prev_send.tv_usec) &&
		diff_send >= g_e.timeout &&
			!g_e.prev_recv.tv_sec && !g_e.prev_recv.tv_usec) ||
		((g_e.prev_recv.tv_sec || g_e.prev_recv.tv_usec) &&
			diff_recv >= g_e.timeout))
		print_statistics(0);
	if (g_e.timeout - diff_send < (size_t)(timeout->tv_sec * 1000000 +
		timeout->tv_usec))
		*timeout = (struct timeval){(g_e.timeout - diff_send) / 1000000,
			(g_e.timeout - diff_send) % 1000000};
	if (g_e.timeout - diff_recv < (size_t)(timeout->tv_sec * 1000000 +
		timeout->tv_usec))
		*timeout = (struct timeval){(g_e.timeout -
		diff_recv) / 1000000, (g_e.timeout - diff_recv) % 1000000};
}
