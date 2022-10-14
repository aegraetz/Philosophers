/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anniegraetz <anniegraetz@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 10:02:55 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/14 11:43:23 by anniegraetz      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static t_phil	**init_phils(t_process *process);
static pthread_mutex_t	*init_forks(t_process *process);
static bool	init_mutex(t_process *process);
static void	alloc_forks(t_phil *phil);

/*Initialise the struct containing the timings of the process. Also triggers the 
initialisation of the philosophers struct and initialises the mutex locks for the forks
Returns 0 if anything doesn't initiliase */
t_process	*init_process(int argc, char **argv, int i)
{
	t_process	*process;

	process = malloc(sizeof(t_process) * 1);
	if (!process)
		return(0);
	process->no_phils = int_atoi(argv[i++]);
	process->t_2_die = int_atoi(argv[i++]);
	process->t_2_eat = int_atoi(argv[i++]);
	process->t_2_sleep = int_atoi(argv[i++]);
	process->eat_freq = -1;
	if (argc - 1 == 5)
		process->eat_freq = int_atoi(argv[i]);
	process->phils = init_phils(process);
	if (!process->phils)
		return(0);
	if(!init_mutex(process))
		return(0);
	process->stop = false;
	return(process);
}

/* Malloc for each philosopher and initialise the struct */
static t_phil	**init_phils(t_process *process)
{
	t_phil			**phils;
	unsigned int	i;

	phils= malloc(sizeof(t_phil) * process->no_phils);
	if (!phils)
	{
		printf("ERROR: unable to allocate memory \n");
		return(error_null(process));
	}
	i = 0;
	while (i < process->no_phils)
	{
		phils[i] = malloc(sizeof(t_phil) * 1);
		if (!phils[i])
			return(error_null(process));
		if (pthread_mutex_init(&phils[i]->dinner_time_lock, 0) != 0)
		{
			printf("ERROR: unable to create mutex \n");
			return(error_null(process));
		}
		phils[i]->process = process;
		phils[i]->id = i;
		phils[i]->fed = 0;
		alloc_forks(phils[i]);
		i++;
	}
	return(phils);
}

/* Initialise the mutex locks for forks, status reporting and stopping the sim */
static bool	init_mutex(t_process *process)
{
	process->fork_locks = init_forks(process);
	if (!process->fork_locks)
		return (false);
	if (pthread_mutex_init(&process->stop_lock, 0) != 0)
	{
		printf("ERROR: unable to create mutext lock \n");
		return(error_null(process));
	}
	if (pthread_mutex_init(&process->write_lock, 0) != 0)
	{
		printf("ERROR: unable to create mutext lock \n");
		return(error_null(process));
	}
	return (true);
}

/* Malloc for each fork and initilise mutexes */
static pthread_mutex_t	*init_forks(t_process *process)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * process->no_phils);
	if (!forks)
	{
		printf("ERROR: unable to allocate memory \n");
		return(error_null(process));
	}
	i = 0;
	while (i < process->no_phils)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			printf("ERROR: unable to create mutext\n");
			return(error_null(process));
		}
		i++;
	}
	return (forks);
}

/*ensures deadlocks are avoided by having the even numbered philosophers 
taking the fork on the left first, forcing the philosopher on the left to wait till
both forks are free and therefore ensuring they all take turns eating*/
static void	alloc_forks(t_phil *phil)
{
	phil->fork[0] = phil->id;
	phil->fork[1] = (phil->id +1) % phil->process->no_phils;
	if (phil->id % 2)
	{
		phil->fork[0] = (phil->id + 1) % phil->process->no_phils;
		phil->fork[1] = phil->id;
	}
}