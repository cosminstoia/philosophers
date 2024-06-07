/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:41:03 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/07 13:47:29 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

// Store the data for each philosopher
typedef struct s_philo
{
	int					index;
	int					left_fork;
	int					right_fork;
	long long			last_meal;
	t_data				*data;
}						t_philo;

// Store the data from the input
typedef struct s_data
{
	int					num_of_philo;
	long long			time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					meal;
	long long			start_time;
	long long			c_time;
	int					dead;
	t_philo				*philo;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*last_meal_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		dead_mutex;
}						t_data;

// Parse input function
void					parse_input(t_data *data, char **argv);

// Threads fucntions
int						init_threads(t_data *data);
long long				get_time_in_ms(void);
int						handel_one_philo(t_philo *philo);
int						check_if_dead(t_philo *philo);
void					destroy_mutex(t_data *data);
void					join_threads(t_data *data, pthread_t *th);
void					ft_usleep(useconds_t microseconds);

// Error function
int						ft_error(char *str);

#endif
