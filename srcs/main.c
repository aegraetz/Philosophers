/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agraetz <agraetz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 09:01:25 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/24 13:49:20 by agraetz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static bool	spaghetti_time(t_process *process);
static void	spaghetti_finished(t_process *process);

int	main(int argc, char **argv)
{
	t_process	*process;

	process = NULL;
	if (argc - 1 < 4 || argc - 1 > 5)
	{
		printf("ERROR: incorrect no of arguments \n");
		return (0);
	}
	if (!input_check(argc, argv))
		return (0);
	process = init_process(argc, argv, 1);
	if (!process)
	{
		printf("ERROR: unable to initialise process \n");
		return (0);
	}
	if (!spaghetti_time(process))
	{
		printf("Something went wrong! \n");
		return (0);
	}
	spaghetti_finished(process);
	return (0);
}

/* Starts the philosophers process. Timer begins and threads 
are created for each philosopher and a death thread for
detecting when a philosopher has died */
static bool	spaghetti_time(t_process *process)
{
	unsigned int	i;

	process->start_time = get_time_in_ms() + (process->no_phils * 2 * 10);
	i = 0;
	while (i < process->no_phils)
	{
		if (pthread_create(&process->phils[i]->thread, NULL,
				&philosophise, process->phils[i]) != 0)
		{
			printf("ERROR: Could not create thread.\n");
			return (false);
		}
		i++;
	}
	if (process->no_phils > 1)
	{
		if (pthread_create(&process->death, NULL, &death, process) != 0)
		{
			printf("ERROR: Could not create thread.\n");
			return (false);
		}
	}
	return (true);
}

/* waits for all threads to be joined then destroys
mutexes and frees allocated mem */
static void	spaghetti_finished(t_process *process)
{
	unsigned int	i;

	i = 0;
	while (i < process->no_phils)
	{
		pthread_join(process->phils[i]->thread, NULL);
		i++;
	}
	if (process->no_phils > 1)
		pthread_join(process->death, NULL);
	destroy_mutexes(process);
	free_process(process);
}
