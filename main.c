/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:22:24 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/07 23:59:28 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	init_threads(t_data *data)
{
	int			i;
	pthread_t	*th;
	pthread_t	still_alive;

	th = malloc(data->num_of_philo * sizeof(pthread_t));
	if (th == NULL)
	{
		ft_error("Error: Failed to allocate memory");
		return (EXIT_FAILURE);
	}
	data->philo = malloc(data->num_of_philo * sizeof(t_philo));
	if (data->philo == NULL)
	{
		ft_error("Error: Failed to allocate memory");
		free(th);
		return (EXIT_FAILURE);
	}
	data->forks = malloc(data->num_of_philo * sizeof(pthread_mutex_t));
	if (data->forks == NULL)
	{
		ft_error("Error: Failed to allocate memory");
		free(th);
		free(data->philo);
		return (EXIT_FAILURE);
	}
	data->last_meal_mutex = malloc(data->num_of_philo
			* sizeof(pthread_mutex_t));
	if (data->last_meal_mutex == NULL)
	{
		ft_error("Error: Failed to allocate memory");
		free(th);
		free(data->philo);
		free(data->forks);
		return (EXIT_FAILURE);
	}
	data->start_time = get_time_in_ms();
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
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
			free(th);
			free(data->philo);
			free(data->forks);
			free(data->last_meal_mutex);
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (pthread_create(&still_alive, NULL, &check_if_alive, data) != 0)
	{
		ft_error("Error: Failed to create thread");
		destroy_mutex(data);
		free(th);
		free(data->philo);
		free(data->forks);
		free(data->last_meal_mutex);
		return (EXIT_FAILURE);
	}
	join_threads(data, th, still_alive);
	destroy_mutex(data);
	free(th);
	free(data->philo);
	free(data->forks);
	free(data->last_meal_mutex);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		parse_input(&data, argv);
		if (init_threads(&data))
			return (0);
	}
	else
		ft_error("Wrong number of arguments!");
	return (0);
}
