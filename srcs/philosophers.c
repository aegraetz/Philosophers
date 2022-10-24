/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agraetz <agraetz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 09:49:07 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/24 14:24:26 by agraetz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static	void	*one_phil(t_phil *phil);
static	void	think_time(t_phil *phil, bool t);
static	void	eat_n_sleep(t_phil *phil);

/*philosophers thread - eat, sleep, think. Even numbered
philosophers think first, allowing odd numbered philosophers 
to grab forks and eat first and avoid deadlock.*/
void	*philosophise(void *data)
{
	t_phil	*phil;

	phil = (t_phil *)data;
	if (phil->process->eat_freq == 0)
	{
		printf("eat frequency can not be set to 0\n");
		return (0);
	}
	pthread_mutex_lock(&phil->dinner_time_lock);
	phil->last_fed = phil->process->start_time;
	pthread_mutex_unlock(&phil->dinner_time_lock);
	delay_start(phil->process->start_time);
	if (phil->process->t_2_die == 0)
		return (0);
	if (phil->process->no_phils == 1)
		return (one_phil(phil));
	else if (phil->id % 2)
		think_time(phil, true);
	while (sim_stopped(phil->process) == false)
	{
		eat_n_sleep(phil);
		think_time(phil, false);
	}
	return (0);
}

/*lonely philosopher only has 1 fork and can't eat.
Picks up fork and then waits t_2_die.
Avoids process getting stuck while waiting for another fork*/
static void	*one_phil(t_phil *phil)
{
	pthread_mutex_lock(&phil->process->fork_locks[phil->fork[0]]);
	status_report(phil, false, GOT_FORK_1);
	check_pause(phil->process, phil->process->t_2_die);
	status_report(phil, false, DIED);
	pthread_mutex_unlock(&phil->process->fork_locks[phil->fork[0]]);
	return (0);
}

/*between sleeping and eating, philosopher will think.
Time spent thinking calculated depending on how long since phil's last meal, 
t_2_eat & t_2_die to determine when phil will next be ready to eat*/
static	void	think_time(t_phil *phil, bool t)
{
	time_t	think;

	pthread_mutex_lock(&phil->dinner_time_lock);
	think = (phil->process->t_2_die - (get_time_in_ms()
				- phil->last_fed) - phil->process->t_2_eat) / 2;
	pthread_mutex_unlock(&phil->dinner_time_lock);
	if (think < 0)
		think = 0;
	if (think == 0 && t == true)
		think = 1;
	if (think > 600)
		think = 200;
	if (t == false)
		status_report(phil, false, THINKING);
	check_pause(phil->process, think);
}

/*when it's time to eat, philosopher will wait for forks
and then eat for set amount of time. When meal time over,
philosopher sleeps and fork mutexes are unlocked for others
to use*/
static void	eat_n_sleep(t_phil *phil)
{
	pthread_mutex_lock(&phil->process->fork_locks[phil->fork[0]]);
	status_report(phil, false, GOT_FORK_1);
	pthread_mutex_lock(&phil->process->fork_locks[phil->fork[1]]);
	status_report(phil, false, GOT_FORK_2);
	status_report(phil, false, EATING);
	pthread_mutex_lock(&phil->dinner_time_lock);
	phil->last_fed = get_time_in_ms();
	pthread_mutex_unlock(&phil->dinner_time_lock);
	check_pause(phil->process, phil->process->t_2_eat);
	if (sim_stopped(phil->process) == false)
	{
		pthread_mutex_lock(&phil->dinner_time_lock);
		phil->fed += 1;
		pthread_mutex_unlock(&phil->dinner_time_lock);
	}
	status_report(phil, false, SLEEPING);
	pthread_mutex_unlock(&phil->process->fork_locks[phil->fork[1]]);
	pthread_mutex_unlock(&phil->process->fork_locks[phil->fork[0]]);
	check_pause(phil->process, phil->process->t_2_sleep);
}
