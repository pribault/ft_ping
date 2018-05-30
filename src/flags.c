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
