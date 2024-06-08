/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/08 12:04:16 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Check if all philosophers are alive
void	*check_if_alive(void *arg)
{
	t_data	*data;
	int		i;
	int		j;

	data = (t_data *)arg;
	j = 0;
	while (data->meal == -1 || j < data->meal)
	{
		i = 0;
		while (i < data->num_of_philo)
		{
			if (check_if_dead(&data->philo[i]))
				return (NULL);
			i++;
		}
		j++;
	}
	return (NULL);
}

// Function that handle the case where there is only one philosopher
int	handel_one_philo(t_philo *philo)
{
	if (philo->data->num_of_philo == 1)
	{
		printf("%lld %d is waiting for forks\n", philo->data->c_time,
			philo->index);
		usleep(philo->data->time_to_die);
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
			return (1);
		}
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (1);
	}
	return (0);
}

// Routine for each philosopher
void	*routine(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	i = 0;
	while ((philo->data->dead == 0) && (philo->data->meal == -1
			|| i < philo->data->meal))
	{
		if (check_if_dead(philo))
			break ;
		if (!eat(philo) || philo->data->dead == 1)
			break ;
		i++;
		if (philo->data->dead == 0 && (philo->data->meal == -1
				|| i < philo->data->meal))
			sleep_and_think(philo);
		if (philo->data->meal != -1 && i >= philo->data->meal)
			break ;
	}
	return (NULL);
}
