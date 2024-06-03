/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/03 10:27:21 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function called for each thread
static void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		printf("%d is thinking\n", philo->index);
		usleep(1000);
		printf("%d is eating\n", philo->index);
		usleep(1000);
		printf("%d is sleeping\n", philo->index);
		usleep(1000);
	}
	free(philo);
	return (NULL);
}

// Initialize a thread and calls routine function for each philosopher
void	init_threads(t_data *data)
{
	int			i;
	pthread_t	th[data->num_of_philo];

	i = 0;
	data->philo = malloc(data->num_of_philo * sizeof(t_philo));
	while (i < data->num_of_philo)
	{
		if (!data->philo)
			ft_error("Failed to allocate memory");
		data->philo[i].index = i + 1;
		data->philo[i].data = data;
		if (pthread_create(&th[i], NULL, &routine, &data->philo[i]) != 0)
			ft_error("Error: Failed to create thread");
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_join(th[i], NULL) != 0)
			ft_error("Error: Failed to join thread");
		i++;
	}
}
