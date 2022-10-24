# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agraetz <agraetz@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/14 11:20:02 by anniegraetz       #+#    #+#              #
#    Updated: 2022/10/24 13:58:39 by agraetz          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror  -pthread -Iheader/

SRCS = srcs/main.c \
		srcs/checks.c \
		srcs/death.c \
		srcs/end.c \
		srcs/init.c \
		srcs/philosophers.c \
		srcs/"times_&_reporting.c"

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean: fclean

fclean:
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re                                                                                                                                                                                                                                                                                                    