/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anniegraetz <anniegraetz@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 09:22:04 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/10 10:12:00 by anniegraetz      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHIL_H
# define PHIL_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_phil
{
	pthread_t		thread;
	unsigned int	id;
	unsigned int	fed;
	unsigned int	fork[2];
	pthread_mutex_t	dinner_time_lock;
	time_t			last_fed;
	t_process		*process;
}	t_phil;

typedef struct s_process
{
	time_t			start_time;
	unsigned int	no_phils;
	pthread_t		death;
	time_t			t_2_die;
	time_t			t_2_eat;
	time_t			t_2_sleep;
	int				eat_freq;
	bool			stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_phil			**phils;
}	t_process;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING= 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}  t_status;

# endif