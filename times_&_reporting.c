/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times_&_reporting.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anniegraetz <anniegraetz@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 10:02:00 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/13 12:00:42 by anniegraetz      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	print_to_console(t_phil *phil, char *stats);

/*fetch the current time and converts to milliseconds*/
time_t	get_time_in_ms()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*delays beginning sim so that all threads execute at once*/
void	delay_start(time_t start)
{
	while(get_time_in_ms() < start)
		continue;
}

/*pauses the philosopher thread mid process to check that the 
simulation is still running. If it's not then the process is stopped.*/
void	check_pause(t_process *process, time_t action)
{
	time_t	next;

	next = get_time_in_ms() + action;
	while (get_time_in_ms() < next)
	{
		if (sim_stopped(process))
			break;
		usleep(100); //suspends execution of thread for 100 microseconds
	}
}

/*reports the status of each philosopher to the console
mutex locks included to avoid messages getting mixed up together*/
void	status_report(t_phil *phil, bool death_stats, t_status status)
{
	pthread_mutext_lock(&phil->process->write_lock);
	if (sim_stopped(phil->process) == true && death_stats == false)
	{
		pthread_mutex_unlock(&phil->process->write_lock);
		return;
	}
	if(status == DIED)
		print_to_console(phil, "died");
	else if (status == EATING)
		print_to_console(phil, "is stuffing his face");
	else if (status == SLEEPING)
		print_to_console(phil, "is sleeping philosophical dreams");
	else if (status == THINKING)
		print_to_console(phil, "is thinking philosophical thoughts");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_to_console(phil, "has grabbed a fork");
	pthread_mutex_unlock(&phil->process->write_lock);
}

static void	print_to_console(t_phil *phil, char *stats)
{
	printf("%ld %d %s \n", get_time_in_ms() 
			- phil->process->start_time, phil->id + 1, stats);
}