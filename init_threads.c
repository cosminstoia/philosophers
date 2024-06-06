/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/06 17:07:40 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// More precise usleep function
static void	ft_usleep(useconds_t microseconds)
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

// Initialize variables
static void	init_variables(t_data *data, int i)
{
	data->c_time = 0;
	data->dead = 0;
	data->philo[i].last_meal = 0;
	data->philo[i].index = i + 1;
	data->philo[i].left_fork = i;
	data->philo[i].right_fork = (i + 1) % data->num_of_philo;
	data->philo[i].data = data;
}

// Eating routine for each philo
static int	eat(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->data->forks[philo->left_fork]) != 0
		|| handel_one_philo(philo))
		return (0);
	if (pthread_mutex_lock(&philo->data->forks[philo->right_fork]) != 0)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return (0);
	}
	if (check_if_dead(philo))
		return (0);
	else
	{
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		pthread_mutex_lock(&philo->data->last_meal_mutex[philo->index - 1]);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d has taken a fork\n", philo->data->c_time, philo->index);
		printf("%lld %d has taken a fork\n", philo->data->c_time, philo->index);
		printf("%lld %d is eating\n", philo->data->c_time, philo->index);
		pthread_mutex_unlock(&philo->data->print_mutex);
		philo->last_meal = philo->data->c_time;
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->last_meal_mutex[philo->index - 1]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	}
	return (1);
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
		if (!eat(philo))
			return (NULL);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d is thinking\n", philo->data->c_time, philo->index);
		printf("%lld %d is sleeping\n", philo->data->c_time, philo->index);
		pthread_mutex_unlock(&philo->data->print_mutex);
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
	pthread_mutex_init(&data->print_mutex, NULL);
	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_init(&data->last_meal_mutex[i], NULL);
		pthread_mutex_init(&data->forks[i], NULL);
		init_variables(data, i);
		if (pthread_create(&th[i], NULL, &routine, &data->philo[i]) != 0)
			ft_error("Error: Failed to create thread");
		i++;
	}
	join_threads(data, th);
	destroy_mutex(data);
	free(th);
}
