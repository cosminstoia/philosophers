/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:41:03 by cstoia            #+#    #+#             */
/*   Updated: 2024/06/12 18:52:05 by cstoia           ###   ########.fr       */
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
	int					meal_count;
	t_data				*data;
}						t_philo;

// Store the data
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
	pthread_mutex_t		start_mutex;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		check_dead_mutex;
	pthread_mutex_t		c_time_mutex;
}						t_data;

// Parse input function
int						parse_input(t_data *data, char **argv);

// Threads and mutexes fucntions
int						init_threads(t_data *data);
int						join_threads(t_data *data, pthread_t *th,
							pthread_t still_alive);
void					destroy_mutex(t_data *data);

// Thread`s routine functions
void					*routine(void *arg);
int						philo_eat(t_philo *philo);
void					philo_think(t_philo *philo);
void					philo_sleep(t_philo *philo);
int						check_if_dead(t_philo *philo);
int						count_meals(t_data *data);
void					*check_if_alive(void *arg);

// Utility functions
u_int64_t				get_time_in_ms(void);
int						ft_usleep(unsigned int time_in_microseconds);

// Free memory
void					free_memory(t_data *data, pthread_t *th);

// Error function
int						ft_error(char *str);

#endif
