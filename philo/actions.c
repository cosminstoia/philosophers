/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:57:45 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/12 19:10:28 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_and_update_status(t_philo *philo, const char *status)
{
	pthread_mutex_lock(&philo->data->check_dead_mutex);
	if (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		pthread_mutex_lock(&philo->data->c_time_mutex);
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		printf("%lld %d %s\n", philo->data->c_time, philo->index, status);
		pthread_mutex_unlock(&philo->data->c_time_mutex);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	pthread_mutex_unlock(&philo->data->check_dead_mutex);
}

static int	grab_forks(t_philo *philo, int first_fork, int second_fork)
{
	int	temp;

	if (first_fork > second_fork)
	{
		temp = first_fork;
		first_fork = second_fork;
		second_fork = temp;
	}
	if (pthread_mutex_lock(&philo->data->forks[first_fork]) != 0)
		return (EXIT_FAILURE);
	if (first_fork == second_fork
		|| pthread_mutex_lock(&philo->data->forks[second_fork]) != 0)
	{
		print_and_update_status(philo, "has taken a fork");
		pthread_mutex_unlock(&philo->data->forks[first_fork]);
		return (EXIT_FAILURE);
	}
	else if (philo->data->num_of_philo < 1)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	}
	return (EXIT_SUCCESS);
}

int	philo_eat(t_philo *philo)
{
	if (grab_forks(philo, philo->left_fork, philo->right_fork))
		return (EXIT_FAILURE);
	if (philo->data->num_of_philo > 1)
	{
		pthread_mutex_lock(&philo->data->last_meal_mutex[philo->index - 1]);
		pthread_mutex_lock(&philo->data->c_time_mutex);
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		philo->last_meal = philo->data->c_time;
		pthread_mutex_unlock(&philo->data->c_time_mutex);
		print_and_update_status(philo, "has taken a fork");
		print_and_update_status(philo, "has taken a fork");
		print_and_update_status(philo, "is eating");
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_lock(&philo->data->meal_mutex);
		philo->meal_count++;
		pthread_mutex_unlock(&philo->data->meal_mutex);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->last_meal_mutex[philo->index - 1]);
	}
	return (EXIT_SUCCESS);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->check_dead_mutex);
	if (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		pthread_mutex_lock(&philo->data->c_time_mutex);
		printf("%lld %d is sleeping\n", get_time_in_ms()
			- philo->data->start_time, philo->index);
		pthread_mutex_unlock(&philo->data->c_time_mutex);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	pthread_mutex_unlock(&philo->data->check_dead_mutex);
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->check_dead_mutex);
	if (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		pthread_mutex_lock(&philo->data->c_time_mutex);
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		printf("%lld %d is thinking\n", philo->data->c_time, philo->index);
		pthread_mutex_unlock(&philo->data->c_time_mutex);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	pthread_mutex_unlock(&philo->data->check_dead_mutex);
}
