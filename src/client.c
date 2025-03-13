/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cari <cari@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 22:46:45 by cari              #+#    #+#             */
/*   Updated: 2025/03/13 22:46:48 by cari             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	*g_ptr;

void	comm_init(t_package *package, int pid, void *message)
{
	package->pid = pid;
	package->message = (char *)message;
	package->size = (int) ft_strlen(message);
	package->status = 0x01;
}

void	send_length(t_package *package)
{
	if (package->bit_count < 32)
	{
		if (package->size & (1 << package->bit_count))
			kill(package->pid, SIGUSR1);
		else
			kill(package->pid, SIGUSR2);
		package->bit_count++;
	}
	if (package->bit_count == 32)
	{
		package->status = 0x02;
		package->bit_count = 0;
	}
}

void	send_message(t_package *package)
{
	if (package->size == 0)
		kill(package->pid, SIGUSR1);
	if (package->index < package->size)
	{
		if (package->bit_count < 8)
		{
			if (package->message[package->index] & (1 << package->bit_count))
				kill(package->pid, SIGUSR1);
			else
				kill(package->pid, SIGUSR2);
			package->bit_count++;
		}
		if (package->bit_count == 8)
		{
			package->index++;
			package->bit_count = 0;
		}
	}
	if (package->index == package->size)
		package->status = 0x03;
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static t_package	package;

	(void)context;
	(void)signum;
	if (package.status == 0x00)
		comm_init(&package, info->si_pid, g_ptr);
	if (package.status == 0x01)
		send_length(&package);
	else if (package.status == 0x02)
		send_message(&package);
	else if (package.status == 0x03)
	{
		ft_printf("Message sent\n");
		exit(0);
	}
}

int	main(int argc, char const *argv[])
{
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_printf("Usage: %s <server-pid> <message>\n", argv[0]);
		return (1);
	}
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	g_ptr = (void *)argv[2];
	kill(ft_atoi(argv[1]), SIGUSR1);
	while (1)
		pause();
	return (0);
}
