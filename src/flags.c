/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/05/30 09:14:42 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

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

void	get_packet_size(t_env *env, char **args, int n)
{
	(void)n;
	env->packet_size = ft_atou(args[0]);
}
