/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 11:43:15 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/07 20:09:04 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Gets the current time in milliseconds
long long	get_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

// Function that handle the case where there is only one philosopher
int	handel_one_philo(t_philo *philo)
{
	if (philo->data->num_of_philo == 1)
	{
		printf("%lld %d is waiting for forks\n", philo->data->c_time,
			philo->index);
		usleep(philo->data->time_to_die);
		printf("%lld %d died\n", philo->data->c_time, philo->index);
		return (1);
	}
	return (0);
}

// Function to check if a philosopher is dead
int	check_if_dead(t_philo *philo)
{
	long long	current_time;

	current_time = get_time_in_ms() - philo->data->start_time;
	if (philo->data->dead == 0 && current_time
		- philo->last_meal > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		if (philo->data->dead == 0)
		{
			philo->data->dead = 1;
			printf("%lld %d died\n", current_time, philo->index);
		}
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (1);
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
		pthread_mutex_destroy(&data->last_meal_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
}

// Fucntion used to join the threads
void	join_threads(t_data *data, pthread_t *th, pthread_t still_alive)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_join(th[i], NULL) != 0)
			ft_error("Error: Failed to join thread");
		i++;
	}
	if (pthread_join(still_alive, NULL) != 0)
		ft_error("Error: Failed to join thread");
}
