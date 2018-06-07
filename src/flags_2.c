/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 00:58:23 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	get_packet_size(t_env *env, char **args, int n)
{
	(void)n;
	env->packet_size = ft_atou(args[0]);
}

void	get_count(t_env *env, char **args, int n)
{
	(void)n;
	env->count = ft_atou(args[0]);
}

void	get_timeout(t_env *env, char **args, int n)
{
	(void)n;
	env->timeout = ft_atof(args[0]) * 1000000;
}

void	print_usage(void)
{
	printf("Usage: ft_ping [-hv] [--help] [--interval interval]\n");
	printf("               [--ttl ttl] [--packetsize packet_size]\n");
	printf("               [--count count] [--timeout seconds]\n");
	exit(1);
}
