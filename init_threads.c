/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/03 15:57:31 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function to get the time
static long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

// Function called for each thread
static void	*routine(void *arg)
{
	t_philo		*philo;
	int			i;
	long long	last_meal_time;

	last_meal_time = get_time_in_ms();
	i = 0;
	philo = (t_philo *)arg;
	if (philo->data->num_of_philo == 1)
	{
		printf("%lld %d is waiting for forks\n", get_time_in_ms(),
			philo->index);
		usleep(philo->data->time_to_die);
		printf("%lld %d died\n", get_time_in_ms(), philo->index);
		return (NULL);
	}
	while (philo->data->limit_meals == -1 || i < philo->data->limit_meals)
	{
		if (get_time_in_ms() - last_meal_time > philo->data->time_to_die)
		{
			printf("%lld %d died\n", get_time_in_ms(), philo->index);
			return (NULL);
		}
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		printf("%lld %d has taken a fork\n", get_time_in_ms(), philo->index);
		printf("%lld %d has taken a fork\n", get_time_in_ms(), philo->index);
		printf("%lld %d is eating\n", get_time_in_ms(), philo->index);
		usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		printf("%lld %d is thinking\n", get_time_in_ms(), philo->index);
		usleep(10000);
		printf("%lld %d is sleeping\n", get_time_in_ms(), philo->index);
		usleep(philo->data->time_to_sleep);
		i++;
	}
	return (NULL);
}

// Initialize a thread and calls routine function for each philosopher
void	init_threads(t_data *data)
{
	int			i;
	pthread_t	th[data->num_of_philo];

	data->philo = malloc(data->num_of_philo * sizeof(t_philo));
	data->forks = malloc(data->num_of_philo * sizeof(pthread_mutex_t));
	if (!data->forks || !data->philo)
		ft_error("Failed to allocate memory");
	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		data->philo[i].index = i + 1;
		data->philo[i].left_fork = i;
		data->philo[i].right_fork = (i + 1) % data->num_of_philo;
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
	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}
