/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 11:43:15 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/08 11:00:16 by cstoia           ###   ########.fr       */
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
long long	get_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

// More precise usleep function
void	ft_usleep(useconds_t microseconds)
{
	struct timeval	end_time;
	struct timeval	start;

	gettimeofday(&start, NULL);
	end_time.tv_sec = start.tv_sec + microseconds / 1000000;
	end_time.tv_usec = start.tv_usec + microseconds % 1000000;
	if (end_time.tv_usec >= 1000000)
	{
		end_time.tv_sec += 1;
		end_time.tv_usec -= 1000000;
	}
	while ((start.tv_sec < end_time.tv_sec) || (start.tv_sec == end_time.tv_sec
			&& start.tv_usec < end_time.tv_usec))
	{
		usleep(1000);
		gettimeofday(&start, NULL);
	}
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
