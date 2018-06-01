/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 01:33:50 by pribault          #+#    #+#             */
/*   Updated: 2018/06/02 01:53:26 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_float2(double nbr, uint8_t power, int *len)
{
	long long	n;
	size_t		l;

	n = (long long)(nbr * ft_power(10, power)) %
		ft_power(10, power);
	l = number_len(n, 10);
	while (l++ < power)
		*len += print_char('0');
	print_nbr_base(n, len, "0123456789", 10);
}

void	if_forest_float(va_list va, t_param p, int *len)
{
	double	nbr;

	nbr = va_arg(va, double);
	*len += number_len((int64_t)nbr, 10);
	print_nbr_base((long long)nbr, len, "0123456789", 10);
	if (nbr != (int64_t)nbr)
	{
		*len += print_char('.');
		if (p.bool)
			print_float2(nbr, p.prec, len);
		else
			print_float2(nbr, 6, len);
	}
}
