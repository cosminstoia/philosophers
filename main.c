/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:22:24 by cstoia            #+#    #+#             */
/*   Updated: 2024/05/31 12:34:42 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *str)
{
	printf("%s\n", str);
	return (EXIT_FAILURE);
}

void	*routine(void *arg)
{
	(void)arg;
	printf("Thread has been created\n");
	return (NULL);
}
int	main(int argc, char **argv)
{
	int i;
	int num_of_philo;

	if (argc == 2)
	{
		num_of_philo = ft_atoi(argv[1]);
		pthread_t th[num_of_philo];

		i = 0;
		while (i < num_of_philo)
		{
			if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
				ft_error("Failed to create thread\n");
			i++;
		}
		i = 0;
		while (i < num_of_philo)
		{
			if (pthread_join(th[i], NULL) != 0)
				ft_error("Failed to join thread\n");
			i++;
		}
		printf("Threads are joined\n");
	}
	return (0);
}