/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/04 11:47:38 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function called for each thread
static void	*routine(void *arg)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = (t_philo *)arg;
	philo->data->last_meal_time = get_time_in_ms();
	while (philo->data->limit_meals == -1 || i < philo->data->limit_meals)
	{
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		handel_one_philo(philo);
		check_if_dead(philo);
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		printf("%lld %d has taken a fork\n", philo->data->c_time, philo->index);
		printf("%lld %d has taken a fork\n", philo->data->c_time, philo->index);
		printf("%lld %d is eating\n", philo->data->c_time, philo->index);
		usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		printf("%lld %d is thinking\n", philo->data->c_time, philo->index);
		printf("%lld %d is sleeping\n", philo->data->c_time, philo->index);
		usleep(philo->data->time_to_sleep);
		i++;
	}
	return (NULL);
}

// Initialize a thread and calls routine function for each philosopher
void	init_threads(t_data *data)
{
	int			i;
	pthread_t	*th;

	th = malloc(data->num_of_philo * sizeof(pthread_t));
	data->philo = malloc(data->num_of_philo * sizeof(t_philo));
	data->forks = malloc(data->num_of_philo * sizeof(pthread_mutex_t));
	if (!data->forks || !data->philo || !th)
		ft_error("Failed to allocate memory");
	data->start_time = get_time_in_ms();
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
	join_threads(data, th);
	destroy_mutex(data);
}
