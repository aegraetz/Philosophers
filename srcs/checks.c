/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agraetz <agraetz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 10:25:13 by anniegraetz       #+#    #+#             */
/*   Updated: 2022/10/24 13:49:17 by agraetz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

int			int_atoi(char *str);
static bool	only_nums(char *str);

bool	input_check(int argc, char **argv)
{
	int	i;
	int	nb;

	i = 1;
	while (i < argc)
	{
		if (!only_nums(argv[i]))
		{
			printf("ERROR: input must be numeric\n");
			return (false);
		}
		nb = int_atoi(argv[i]);
		if (i == 1 && (nb <= 0 || nb > 250))
		{
			printf("ERROR: must be between 1 and 250 philosophers\n");
			return (false);
		}
		if (i != 1 && nb == -1)
		{
			printf("ERROR: must be a valid unsigned integer\n");
			return (false);
		}
		i++;
	}
	return (true);
}

/*Converts a digit only string into a positive integer*/
int	int_atoi(char *str)
{
	unsigned long long int	nb;
	int						i;

	i = 0;
	nb = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return ((int)nb);
}

/*checks that the string contains only digits between 0 - 9 */
static bool	only_nums(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}
