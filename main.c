/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:22:24 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/03 15:18:47 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		parse_input(&data, argv);
		init_threads(&data);
	}
	else
		ft_error("Wrong number of arguments!");
	return (0);
}
