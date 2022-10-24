/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agraetz <agraetz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 09:05:37 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/24 13:49:17 by agraetz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	stop_simulation_flag(t_process *process, bool state);
static bool	process_ender(t_process *process);
static bool	eternal_sleep(t_phil *phil);
bool		sim_stopped(t_process *process);

/*a thread to check whether a philosopher must die or 
whether they have eaten enough.*/
void	*death(void *data)
{
	t_process	*process;

	process = (t_process *)data;
	if (process->eat_freq == 0)
		return (0);
	stop_simulation_flag(process, false);
	delay_start(process->start_time);
	while (true)
	{
		if (process_ender(process) == true)
			return (0);
		usleep(1000);
	}
	return (0);
}

/*the death thread sets whether this function is triggered as true because
something has occured to end the simulation */
static void	stop_simulation_flag(t_process *process, bool state)
{
	pthread_mutex_lock(&process->stop_lock);
		process->stop = state;
	pthread_mutex_unlock(&process->stop_lock);
}

/*checks the status of each philosopher to see whether they have 
either eaten enough or they need to die. Returns false unless 
all end conditions are met*/
static bool	process_ender(t_process *process)
{
	unsigned int	i;
	bool			full;

	full = true;
	i = 0;
	while (i < process->no_phils)
	{
		pthread_mutex_lock(&process->phils[i]->dinner_time_lock);
		if (eternal_sleep(process->phils[i]))
			return (true);
		if (process->eat_freq != 1)
			if (process->phils[i]->fed
				< (unsigned int)process->eat_freq)
				full = false;
		pthread_mutex_unlock(&process->phils[i]->dinner_time_lock);
		i++;
	}
	if (process->eat_freq != -1 && full == true)
	{
		stop_simulation_flag(process, true);
		return (true);
	}
	return (false);
}

/*Compares time since last fed with t_2_die. if it has been longer or equal to 
t_2_die since philosopher last ate then they must be killed and the simulation
stop flag is set. Returns true if a philosopher was killed.*/
static bool	eternal_sleep(t_phil *phil)
{
	time_t	time;

	time = get_time_in_ms();
	if ((time - phil->last_fed) >= phil->process->t_2_die)
	{
		stop_simulation_flag(phil->process, true);
		status_report(phil, true, DIED);
		pthread_mutex_unlock(&phil->dinner_time_lock);
		return (true);
	}
	return (false);
}

/*a function to check whether the simulation has finished.*/
bool	sim_stopped(t_process *process)
{
	bool	flag;

	flag = false;
	pthread_mutex_lock(&process->stop_lock);
	if (process->stop == true)
		flag = true;
	pthread_mutex_unlock(&process->stop_lock);
	return (flag);
}
