/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:41:03 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/04 11:47:39 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

// ./philo 5 800 200 200 [5]
typedef struct s_data	t_data;

// Store the data for each philosopher
typedef struct s_philo
{
	int					index;
	int					last_meal;
	int					left_fork;
	int					right_fork;
	t_data				*data;
}						t_philo;

// Store the data from the input
typedef struct s_data
{
	int					num_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					limit_meals;
	long long			start_time;
	long long			last_meal_time;
	long long			c_time;
	pthread_mutex_t		*forks;
	t_philo				*philo;
}						t_data;

// Parse input function
void					parse_input(t_data *data, char **argv);

// Threads fucntions
void					init_threads(t_data *data);
long long				get_time_in_ms(void);
void					*handel_one_philo(t_philo *philo);
void					*check_if_dead(t_philo *philo);
void					destroy_mutex(t_data *data);
void					join_threads(t_data *data, pthread_t *th);

// Error function
int						ft_error(char *str);

#endif