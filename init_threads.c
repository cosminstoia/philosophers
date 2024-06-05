/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/05 20:05:14 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// More precise usleep function
static int	ft_usleep(useconds_t microseconds)
{
	long			elapsed;
	struct timeval	start;
	struct timeval	end;

	if (microseconds < 0)
		return (-1);
	if (gettimeofday(&start, NULL) != 0)
		return (-1);
	elapsed = 0;
	while (elapsed < microseconds)
	{
		usleep(10);
		if (gettimeofday(&end, NULL) != 0)
			return (-1);
		elapsed = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec
				- start.tv_usec);
	}
	return (0);
}

// Function called for each thread
static void	*routine(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	i = 0;
	while (!philo->data->dead && (philo->data->meal == -1
			|| i < philo->data->meal))
	{
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		if (handel_one_philo(philo) || check_if_dead(philo))
			return (NULL);
		if (pthread_mutex_lock(&philo->data->forks[philo->left_fork]) != 0)
			return (NULL);
		if (pthread_mutex_lock(&philo->data->forks[philo->right_fork]) != 0)
		{
			pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
			return (NULL);
		}
		pthread_mutex_lock(&philo->data->last_meal_mutex[philo->index - 1]);
		printf("%lld %d has taken a fork\n", philo->data->c_time, philo->index);
		printf("%lld %d has taken a fork\n", philo->data->c_time, philo->index);
		printf("%lld %d is eating\n", philo->data->c_time, philo->index);
		philo->last_meal = philo->data->c_time;
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->last_meal_mutex[philo->index - 1]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		printf("%lld %d is thinking\n", philo->data->c_time, philo->index);
		printf("%lld %d is sleeping\n", philo->data->c_time, philo->index);
		ft_usleep(philo->data->time_to_sleep);
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
	data->last_meal_mutex = malloc(data->num_of_philo
			* sizeof(pthread_mutex_t));
	if (!data->forks || !data->philo || !th)
		ft_error("Error: Failed to allocate memory");
	data->start_time = get_time_in_ms();
	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_init(&data->last_meal_mutex[i], NULL);
		pthread_mutex_init(&data->forks[i], NULL);
		data->philo[i].last_meal = 0;
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
	free(th);
}
