/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/05/31 17:11:41 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*routine(void *arg)
{
	int	philo_index;

	philo_index = *((int *)arg);
	printf("%d is eating\n", philo_index);
	return (NULL);
}

// Initialize a thread and calls routine function for each philosopher
void	init_threads(t_data *data)
{
	int			i;
	pthread_t	th[data->num_of_philo];
	int			index[data->num_of_philo];

	i = 0;
	while (i < data->num_of_philo)
	{
		index[i] = i + 1;
		if (pthread_create(&th[i], NULL, &routine, &index[i]) != 0)
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
