/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agraetz <agraetz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 10:00:04 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/24 13:49:18 by agraetz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

/*destory all mutexes*/
void	destroy_mutexes(t_process *process)
{
	unsigned int	i;

	i = 0;
	while (i < process->no_phils)
	{
		pthread_mutex_destroy(&process->fork_locks[i]);
		pthread_mutex_destroy(&process->phils[i]->dinner_time_lock);
		i++;
	}
	pthread_mutex_destroy(&process->write_lock);
	pthread_mutex_destroy(&process->stop_lock);
}

/*free all memory*/
void	*free_process(t_process *process)
{
	unsigned int	i;

	if (!process)
		return (0);
	if (process->fork_locks != NULL)
		free(process->fork_locks);
	if (process->phils != NULL)
	{
		i = 0;
		while (i < process->no_phils)
		{
			if (process->phils[i] != NULL)
				free (process->phils[i]);
			i++;
		}
		free(process->phils);
	}
	free(process);
	return (0);
}

void	*error_null(t_process *process)
{
	if (process != NULL)
		free_process(process);
	return (NULL);
}
