/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 11:43:15 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/17 15:53:42 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function to handle error massage
int	ft_error(char *str)
{
	printf("%s\n", str);
	return (EXIT_FAILURE);
}

// Gets the current time in milliseconds
u_int64_t	get_time_in_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (u_int64_t)(1000)) + (tv.tv_usec / 1000));
}

// More precise usleep function
int	ft_usleep(unsigned int time_in_microseconds)
{
	long long	start;

	start = get_time_in_ms();
	while (get_time_in_ms() - start < time_in_microseconds)
		usleep(200);
	return (0);
}

// Function used to destroy each mutex
void	destroy_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->last_meal_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->check_dead_mutex);
	pthread_mutex_destroy(&data->start_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->c_time_mutex);
}

// Fucntion used to join the threads
int	join_threads(t_data *data, pthread_t *th, pthread_t still_alive)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			ft_error("Error: Failed to join thread");
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (pthread_join(still_alive, NULL) != 0)
	{
		ft_error("Error: Failed to join thread");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
