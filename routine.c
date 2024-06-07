/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:57:45 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/07 20:12:17 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Eating routine for each philo
static int	eat(t_philo *philo)
{
	if (handel_one_philo(philo))
		return (0);
	int first_fork, second_fork;
	if (philo->index % 2 == 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
		ft_usleep(100);
	}
	else
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	if (pthread_mutex_lock(&philo->data->forks[second_fork]) != 0)
		return (0);
	if (pthread_mutex_lock(&philo->data->forks[first_fork]) != 0)
	{
		pthread_mutex_unlock(&philo->data->forks[second_fork]);
		return (0);
	}
	else if (!check_if_dead(philo) && philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->data->last_meal_mutex[philo->index - 1]);
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		if (philo->data->dead == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			philo->data->c_time = get_time_in_ms() - philo->data->start_time;
			printf("%lld %d has taken a fork\n", philo->data->c_time,
				philo->index);
			pthread_mutex_unlock(&philo->data->print_mutex);
		}
		if (philo->data->dead == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			philo->data->c_time = get_time_in_ms() - philo->data->start_time;
			printf("%lld %d has taken a fork\n", philo->data->c_time,
				philo->index);
			pthread_mutex_unlock(&philo->data->print_mutex);
		}
		if (philo->data->dead == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			philo->data->c_time = get_time_in_ms() - philo->data->start_time;
			printf("%lld %d is eating\n", philo->data->c_time, philo->index);
			pthread_mutex_unlock(&philo->data->print_mutex);
		}
		pthread_mutex_unlock(&philo->data->print_mutex);
		philo->last_meal = philo->data->c_time;
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->forks[first_fork]);
		pthread_mutex_unlock(&philo->data->forks[second_fork]);
		pthread_mutex_unlock(&philo->data->last_meal_mutex[philo->index - 1]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[first_fork]);
		pthread_mutex_unlock(&philo->data->forks[second_fork]);
	}
	return (1);
}

// Function called for each thread
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
		if (philo->data->dead == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			philo->data->c_time = get_time_in_ms() - philo->data->start_time;
			if (philo->data->dead == 0)
				printf("%lld %d is sleeping\n", philo->data->c_time,
					philo->index);
			pthread_mutex_unlock(&philo->data->print_mutex);
			ft_usleep(philo->data->time_to_sleep);
		}
		if (philo->data->dead == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			philo->data->c_time = get_time_in_ms() - philo->data->start_time;
			if (philo->data->dead == 0)
				printf("%lld %d is thinking\n", philo->data->c_time,
					philo->index);
			pthread_mutex_unlock(&philo->data->print_mutex);
		}
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->meal != -1 && i >= philo->data->meal)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		i++;
	}
	return (NULL);
}
