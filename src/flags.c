/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/07 23:42:23 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	get_flags(int argc, char **argv)
{
	static t_short_flag	short_flags[] = {
		{'h', (void *)&print_usage},
		{'v', (void *)&get_verbose},
		{'\0', NULL}
	};
	static t_long_flag	long_flags[] = {
		{"help", 0, {0}, (void *)&print_usage},
		{"verbose", 0, {0}, (void *)&get_verbose},
		{"ttl", 1, {PARAM_UNSIGNED}, (void *)&get_ttl},
		{"interval", 1, {PARAM_FLOAT}, (void *)&get_interval},
		{"packetsize", 1, {PARAM_UNSIGNED}, (void *)&get_packet_size},
		{"count", 1, {PARAM_UNSIGNED}, (void *)&get_count},
		{"timeout", 1, {PARAM_FLOAT}, (void *)&get_timeout},
		{NULL, 0, {0}, NULL}
	};

	ft_get_flags(argc, argv, ft_get_flag_array((t_short_flag *)&short_flags,
		(t_long_flag *)&long_flags, (void *)&default_getter), &g_e);
}

void	default_getter(char *s, t_env *env)
{
	if (env->address)
		return (ft_error(2, ERROR_ADDRESS_SET, env->address));
	env->address = s;
}

void	get_verbose(t_env *env, char **args, int n)
{
	(void)args;
	(void)n;
	env->opt ^= OPT_VERBOSE;
}

void	get_ttl(t_env *env, char **args, int n)
{
	(void)n;
	env->ttl = ft_atou(args[0]);
}

void	get_interval(t_env *env, char **args, int n)
{
	(void)n;
	env->interval = ft_atof(args[0]) * 1000000;
	if (env->interval < 200000 && getuid())
		ft_error(2, ERROR_MINIMAL_INTERVAL, NULL);
}
