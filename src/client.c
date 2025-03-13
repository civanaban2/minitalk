/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urmet <urmet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:34:26 by urmet             #+#    #+#             */
/*   Updated: 2025/03/12 21:38:01 by urmet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>

void	*g_ptr;

int	tf_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*s++)
		i++;
	return (i);
}

void	comm_init(t_package *package, int pid, void *message)
{
	package->pid = pid;
	package->message = (char *)message;
	package->size = tf_strlen(message);
	package->status = 0x01;
}

void	send_length(t_package *package)
{
	static int	i;

	if (i < 32)
	{
		if (package->size & (1 << i)){
			kill(package->pid, SIGUSR1);
			printf("1");}
			
		else{
			kill(package->pid, SIGUSR2);
			printf("0");}
		i++;
	}
	if (i == 32)
	{
		package->status = 0x02;
		i = 0;
		printf("sended");
	}
}

void	send_message(t_package *package)
{
	static int	i;
	static int	j;

	if (i < package->size)
	{
		if (j < 8)
		{
			if (package->message[i] & (1 << j))
				kill(package->pid, SIGUSR1);
			else
				kill(package->pid, SIGUSR2);
			j++;
		}
		if (j == 8)
		{
			i++;
			j = 0;
		}
	}
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static t_package	package;

	(void)context;
	(void)signum;
	if (package.status == 0x00){
		comm_init(&package, info->si_pid, g_ptr);
		printf("comm_init\n");}
	if (package.status == 0x01) {
		ft_printf("message length: %d\n", package.size);
		send_length(&package);
		printf("send_length\n");}
	if (package.status == 0x02){
		send_message(&package);
		printf("send_message\n");}
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
