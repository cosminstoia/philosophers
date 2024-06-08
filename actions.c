/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:57:45 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/08 11:47:20 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	grab_forks(t_philo *philo, int first_fork, int second_fork)
{
	ft_usleep(100);
	if (pthread_mutex_lock(&philo->data->forks[second_fork]) != 0)
		return (0);
	if (pthread_mutex_lock(&philo->data->forks[first_fork]) != 0)
	{
		pthread_mutex_unlock(&philo->data->forks[second_fork]);
		return (0);
	}
	return (1);
}

static void	print_and_update_status(t_philo *philo, const char *status)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->data->c_time = get_time_in_ms() - philo->data->start_time;
	if (philo->data->dead == 0)
		printf("%lld %d %s\n", philo->data->c_time, philo->index, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

static void	manage_forks_and_eat(t_philo *philo, int first_fork,
		int second_fork)
{
	if (!grab_forks(philo, first_fork, second_fork))
		return ;
	if (!check_if_dead(philo) && philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->data->last_meal_mutex[philo->index - 1]);
		philo->data->c_time = get_time_in_ms() - philo->data->start_time;
		philo->last_meal = philo->data->c_time;
		print_and_update_status(philo, "has taken a fork");
		print_and_update_status(philo, "has taken a fork");
		print_and_update_status(philo, "is eating");
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
}

int	eat(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	if (handel_one_philo(philo))
		return (0);
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
	manage_forks_and_eat(philo, first_fork, second_fork);
	return (1);
}

// Function to handle sleeping and thinking
void	sleep_and_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->data->c_time = get_time_in_ms() - philo->data->start_time;
	if (philo->data->dead == 0)
		printf("%lld %d is sleeping\n", philo->data->c_time, philo->index);
	pthread_mutex_unlock(&philo->data->print_mutex);
	ft_usleep(philo->data->time_to_sleep);
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->data->c_time = get_time_in_ms() - philo->data->start_time;
	if (philo->data->dead == 0)
		printf("%lld %d is thinking\n", philo->data->c_time, philo->index);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
