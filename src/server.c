/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urmet <urmet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:15:49 by urmet             #+#    #+#             */
/*   Updated: 2025/03/13 02:57:58 by urmet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <stdio.h>

void	comm_init(t_package *package, siginfo_t *info)
{
	package->pid = info->si_pid;
	package->status = 0x01;
}

void	get_length(t_package *package, int signum)
{
	static int	bit_count;

	if (bit_count < 32)
	{
		if (signum == SIGUSR1){
			package->size |= (1 << bit_count);
			ft_printf("1");}
		else if (signum == SIGUSR2){
			package->size &= ~(1 << bit_count);
			ft_printf("0");}
		bit_count++;
	}
	if (bit_count == 32)
	{
		ft_printf("Message size: %d\n", package->size);
		package->message = (char *)malloc(package->size + 1);
		if (!package->message)
		{
			ft_printf("Error: malloc failed\n");
			exit(1);
		}
		bit_count = 0;
		package->status = 0x02;
	}
}

void	get_message(t_package *package, int signum)
{
	static int	bit_count;
	static int		size;

	if (bit_count < 8)
	{
		if (signum == SIGUSR1)
			package->message[size] |= (1 << bit_count);
		else if (signum == SIGUSR2)
			package->message[size] &= ~(1 << bit_count);
		bit_count++;
	}
	if (bit_count == 8)
	{
		size++;
		bit_count = 0;
	}
	if (size == package->size)
	{
		package->message[size] = '\0';
		ft_printf("Received message: %s\n", package->message);
		free(package->message);
		package->message = NULL;
		package->status = 0x00;
		size = 0;
	}
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static t_package	package;

	(void)context;
	if (package.status == 0x00){
		comm_init(&package, info);
		printf("comm_init\n");}
	else if (package.status == 0x01 && package.pid == info->si_pid){
		get_length(&package, signum);
		printf("get_length\n");}
	else if (package.status == 0x02 && package.pid == info->si_pid){
		get_message(&package, signum);
		printf("get_message\n");}
	usleep(200000);
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_printf("Server PID: %d\n", getpid());
	while (1)
		pause();
	return (0);
}
