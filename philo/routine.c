/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:40:28 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/12 18:46:23 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function to count meals for each philosopher
int	count_meals(t_data *data)
{
	int	i;
	int	all_ate_required_meals;

	all_ate_required_meals = 1;
	pthread_mutex_lock(&data->meal_mutex);
	i = 0;
	while (i < data->num_of_philo)
	{
		if (data->philo[i].meal_count < data->meal)
			all_ate_required_meals = 0;
		i++;
	}
	pthread_mutex_unlock(&data->meal_mutex);
	if (all_ate_required_meals)
	{
		pthread_mutex_lock(&data->check_dead_mutex);
		data->dead = 1;
		pthread_mutex_unlock(&data->check_dead_mutex);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

// Function to check if a philosopher is dead
int	check_if_dead(t_philo *philo)
{
	long long	current_time;
	int			dead_flag;
	int			last_meal;

	pthread_mutex_lock(&philo->data->check_dead_mutex);
	dead_flag = philo->data->dead;
	pthread_mutex_unlock(&philo->data->check_dead_mutex);
	pthread_mutex_lock(&philo->data->c_time_mutex);
	current_time = get_time_in_ms() - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->c_time_mutex);
	pthread_mutex_lock(&philo->data->last_meal_mutex[philo->index - 1]);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->data->last_meal_mutex[philo->index - 1]);
	if (dead_flag == 0 && current_time - last_meal > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->check_dead_mutex);
		philo->data->dead = 1;
		pthread_mutex_unlock(&philo->data->check_dead_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d died\n", current_time, philo->index);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// Check if all philosophers are alive
void	*check_if_alive(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->num_of_philo)
		{
			if (check_if_dead(&data->philo[i]))
				return (NULL);
			i++;
		}
		if (data->meal != -1)
		{
			if (!count_meals(data))
				return (NULL);
		}
		usleep(50);
	}
	return (NULL);
}

static int	routine_2(t_philo *philo)
{
	if (philo_eat(philo))
		return (EXIT_FAILURE);
	philo_sleep(philo);
	philo_think(philo);
	if (philo->meal_count == philo->data->meal)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// Routine for each philosopher
void	*routine(void *arg)
{
	t_philo	*philo;
	int		dead_flag;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);
	philo_think(philo);
	if (philo->index % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2);
	while (1)
	{
		pthread_mutex_lock(&philo->data->check_dead_mutex);
		dead_flag = philo->data->dead;
		pthread_mutex_unlock(&philo->data->check_dead_mutex);
		if (dead_flag == 0)
		{
			if (routine_2(philo))
				return (NULL);
		}
		else
			break ;
	}
	return (NULL);
}
