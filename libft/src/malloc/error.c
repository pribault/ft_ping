/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 23:02:19 by pribault          #+#    #+#             */
/*   Updated: 2018/03/31 16:58:46 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prototypes.h"

void	malloc_error(int error, void *param)
{
	char		*debug;
	uint32_t	level;

	if ((debug = ft_getenv(g_env.env, "MALLOC_DEBUG")))
		level = ft_atou(debug);
	else
		level = 1;
	pthread_mutex_unlock(&g_env.mutex);
	if (level & 1)
		ft_error(2, error, param);
	pthread_mutex_lock(&g_env.mutex);
	if (level & 2)
		abort();
}

void	malloc_set_env(char **env)
{
	g_env.env = env;
}
