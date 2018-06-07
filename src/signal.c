/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 23:54:57 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 00:02:46 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_statistics(int signal)
{
	struct timeval	end;

	(void)signal;
	gettimeofday(&end, NULL);
	printf("\n--- %s ft_ping statistics ---\n", g_e.address);
	printf("%lu packets transmitted, %lu received, ", g_e.transmitted,
		g_e.received);
	if (g_e.received + g_e.lost != g_e.transmitted)
		printf("+%lu errors, ", g_e.transmitted - g_e.lost - g_e.received);
	if (g_e.transmitted)
		printf("%lu%% packet loss, ", 100 - ((g_e.received * 100) /
		g_e.transmitted));
	printf("time %lums\n", (g_e.start.tv_sec || g_e.start.tv_usec) ?
		(end.tv_sec - g_e.start.tv_sec) * 1000 +
		(end.tv_usec - g_e.start.tv_usec) / 1000 : 0);
	if (g_e.received)
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3lf ms\n", g_e.min,
		g_e.avg, g_e.max, sqrt(g_e.sum2 / g_e.received -
			(g_e.sum / g_e.received) * (g_e.sum / g_e.received)));
	else
		printf("\n");
	exit(0);
}

void	set_signals(void)
{
	signal(SIGINT, &print_statistics);
}
