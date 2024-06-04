/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 11:43:15 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/04 14:46:35 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function to get the time of the day
long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

// Function that handle the case where there is only one philosopher
void	*handel_one_philo(t_philo *philo)
{
	if (philo->data->num_of_philo == 1)
	{
		printf("%lld %d is waiting for forks\n", philo->data->c_time,
			philo->index);
		usleep(philo->data->time_to_die);
		printf("%lld %d died\n", philo->data->c_time, philo->index);
		return (NULL);
	}
	return (0);
}

// Fucntion to check if a philosopher is dead
void	*check_if_dead(t_philo *philo)
{
	if (get_time_in_ms()
		- philo->data->last_meal_time > philo->data->time_to_die)
	{
		printf("%lld %d died\n", philo->data->c_time, philo->index);
		return (NULL);
	}
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
		i++;
	}
}

// Fucntion used to join the threads
void	join_threads(t_data *data, pthread_t *th)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_join(th[i], NULL) != 0)
			ft_error("Error: Failed to join thread");
		i++;
	}
}
