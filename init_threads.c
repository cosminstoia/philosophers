/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/07 19:32:00 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	*check_if_alive(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_if_dead(philo))
		{
			break ;
		}
		// usleep(10000);
	}
	return (NULL);
}
// Initialize a thread and calls routine function for each philosopher
int	init_threads(t_data *data)
{
	int			i;
	pthread_t	*th;
	pthread_t	still_alive;

	th = malloc(data->num_of_philo * sizeof(pthread_t));
	still_alive = malloc(sizeof(pthread_t));
	data->philo = malloc(data->num_of_philo * sizeof(t_philo));
	data->forks = malloc(data->num_of_philo * sizeof(pthread_mutex_t));
	data->last_meal_mutex = malloc(data->num_of_philo
			* sizeof(pthread_mutex_t));
	if (!data->forks || !data->philo || !th)
		ft_error("Error: Failed to allocate memory");
	data->start_time = get_time_in_ms();
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	if (pthread_create(&still_alive, NULL, &check_if_alive,
			&data->philo[0]) != 0)
	{
		ft_error("Error: Failed to create thread");
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_init(&data->last_meal_mutex[i], NULL);
		pthread_mutex_init(&data->forks[i], NULL);
		init_variables(data, i);
		if (pthread_create(&th[i], NULL, &routine, &data->philo[i]) != 0)
		{
			ft_error("Error: Failed to create thread");
			return (EXIT_FAILURE);
		}
		i++;
	}
	join_threads(data, th, still_alive);
	destroy_mutex(data);
	free(th);
	return (0);
}
