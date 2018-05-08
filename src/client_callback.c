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

void	client_add(t_socket *socket, t_client *client)
{
	(void)socket;
	(void)client;
	ft_printf("client added\n");
}

void	client_del(t_socket *socket, t_client *client)
{
	(void)socket;
	(void)client;
	ft_printf("client removed\n");
}
