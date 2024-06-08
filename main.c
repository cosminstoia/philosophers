/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:22:24 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/08 10:58:12 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		if (parse_input(&data, argv))
			return (EXIT_FAILURE);
		if (init_threads(&data))
			return (EXIT_FAILURE);
	}
	else
	{
		ft_error("Wrong number of arguments!");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
