/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 10:42:50 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/08 12:08:06 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static void	free_memory(t_data *data, pthread_t *th)
{
	if (th != NULL)
	{
		free(th);
		th = NULL;
	}
	if (data->philo != NULL)
	{
		free(data->philo);
		data->philo = NULL;
	}
	if (data->forks != NULL)
	{
		free(data->forks);
		data->forks = NULL;
	}
	if (data->last_meal_mutex != NULL)
	{
		free(data->last_meal_mutex);
		data->last_meal_mutex = NULL;
	}
}

static int	allocate_memory(t_data *data, pthread_t **th)
{
	*th = malloc(data->num_of_philo * sizeof(pthread_t));
	data->philo = malloc(data->num_of_philo * sizeof(t_philo));
	data->forks = malloc(data->num_of_philo * sizeof(pthread_mutex_t));
	data->last_meal_mutex = malloc(data->num_of_philo
			* sizeof(pthread_mutex_t));
	if (!*th || !data->philo || !data->forks || !data->last_meal_mutex)
	{
		ft_error("Error: Failed to allocate memory");
		free_memory(data, *th);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	initialize_threads_and_mutexes(t_data *data, pthread_t *th)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_init(&data->last_meal_mutex[i], NULL);
		pthread_mutex_init(&data->forks[i], NULL);
		init_variables(data, i);
		if (pthread_create(&th[i], NULL, &routine, &data->philo[i]) != 0)
		{
			ft_error("Error: Failed to create thread");
			destroy_mutex(data);
			free_memory(data, th);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_threads(t_data *data)
{
	pthread_t	*th;
	pthread_t	still_alive;

	if (allocate_memory(data, &th) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	data->start_time = get_time_in_ms();
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	if (initialize_threads_and_mutexes(data, th) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (pthread_create(&still_alive, NULL, &check_if_alive, data) != 0)
	{
		ft_error("Error: Failed to create thread");
		destroy_mutex(data);
		free_memory(data, th);
		return (EXIT_FAILURE);
	}
	if (join_threads(data, th, still_alive))
		return (EXIT_FAILURE);
	destroy_mutex(data);
	free_memory(data, th);
	return (EXIT_SUCCESS);
}
