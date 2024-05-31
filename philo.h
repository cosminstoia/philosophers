/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstoia <cstoia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:41:03 by cstoia            #+#    #+#             */
/*   Updated: 2024/05/31 17:12:18 by cstoia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

// ./philo 5 800 200 200 [5]

typedef struct s_data
{
	int	num_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	limit_meals;
}		t_data;

// Parse input function
void	parse_input(t_data *data, char **argv);

// Threads fucntions
void	init_threads(t_data *data);

// Error function
int		ft_error(char *str);

#endif