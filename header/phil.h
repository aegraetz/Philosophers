/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agraetz <agraetz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 09:22:04 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/24 13:49:21 by agraetz          ###   ########.fr       */
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

typedef struct s_phil	t_phil;

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

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING= 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

/* INPUT CHECKS */
bool		input_check(int argc, char **argv);
int			int_atoi(char *str);

/* INITIALISING */
t_process	*init_process(int argc, char **argv, int i);

/* PHILOSOPHER PROCESSES */
void		*philosophise(void *data);

/* TIMINGS & REPORTS */
time_t		get_time_in_ms(void);
void		check_pause(t_process *process, time_t action);
void		delay_start(time_t start);
void		status_report(t_phil *phil, bool death_stats, t_status status);

/* DEATH PROCESSES */
void		*death(void *data);
bool		sim_stopped(t_process *process);

/* END PROCESSES */
void		destroy_mutexes(t_process *process);
void		*free_process(t_process *process);
void		*error_null(t_process *process);

#endif