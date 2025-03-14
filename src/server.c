/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cari <cari@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 22:46:37 by cari              #+#    #+#             */
/*   Updated: 2025/03/13 22:46:40 by cari             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	comm_init(t_package *package, siginfo_t *info)
{
	package->pid = info->si_pid;
	package->status = 0x01;
	package->size = 0;
	package->index = 0;
	package->bit_count = 0;
}

void	get_length(t_package *package, int signum)
{	
	if (package->bit_count < 32)
	{
		if (signum == SIGUSR1)
			package->size |= (1 << package->bit_count);
		else if (signum == SIGUSR2)
			package->size &= ~(1 << package->bit_count);
		package->bit_count++;
	}
	if (package->bit_count == 32)
	{
		package->message = (char *)malloc(package->size + 1);
		if (!package->message)
		{
			ft_printf("Error: malloc failed\n");
			exit(1);
		}
		package->bit_count = 0;
		package->status = 0x02;
	}
}

void	get_message(t_package *package, int signum)
{	
	if (package->bit_count < 8)
	{
		if (signum == SIGUSR1)
			package->message[package->index] |= (1 << package->bit_count);
		else if (signum == SIGUSR2)
			package->message[package->index] &= ~(1 << package->bit_count);
		package->bit_count++;
	}
	if (package->bit_count == 8)
	{
		package->index++;
		package->bit_count = 0;
	}
	if (package->index == package->size)
	{
		package->message[package->index] = '\n';
		write(1, package->message, package->size +1);
		free(package->message);
		package->message = NULL;
		package->status = 0x00;
		package->index = 0;
	}
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static t_package	package;

	(void)context;
	if (info->si_pid == getpid())
	{
		package.status = 0x00;
		free(package.message);
		package.message = NULL;
		return ;
	}
	if (package.status == 0x00)
		comm_init(&package, info);
	else if (package.status == 0x01 && package.pid == info->si_pid)
		get_length(&package, signum);
	else if (package.status == 0x02 && package.pid == info->si_pid)
		get_message(&package, signum);
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;
	int					restarted;

	restarted = 0;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_printf("Server PID: %d\n", getpid());
	while (1)
	{
		if (usleep(1000000) != 0)
			restarted = 0;
		else if (restarted == 0)
		{
			kill(getpid(), SIGUSR1);
			restarted = 1;
		}
	}
	return (0);
}
