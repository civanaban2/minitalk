/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cari <cari@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 01:12:46 by cari              #+#    #+#             */
/*   Updated: 2025/03/12 03:37:42 by cari             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	sig_handler(int signum)
{
	if (signum == SIGUSR1)
		return ;
	else if (signum == SIGUSR2)
		exit(1);
}

void	send_bits(int pid, void *data, int bit_count)
{
	int i;

	i = 0;
	if (bit_count == 8)
	{
		while (i < 8)
		{
			if (*((char *)data) & (1 << i)){
				kill(pid, SIGUSR1);
				ft_printf("1");}
			else{
				kill(pid, SIGUSR2);
				ft_printf("0");}
			i++;
			pause();
		}
	}
	else if (bit_count == sizeof(unsigned long) * 8)
	{
		while (i < bit_count)
		{
			if (*((unsigned long *)data) & (1 << i)){
				kill(pid, SIGUSR1);
				ft_printf("1");}
				
			else{
				kill(pid, SIGUSR2);
				ft_printf("0");}
			i++;
			pause();
		}
	}
}

int main(int argc, char **argv)
{
	int pid;
	unsigned long size;

	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);
	if (argc != 3)
	{
		ft_printf("Usage: %s <server-pid> <message>\n", argv[0]);
		return 1;
	}
	size = ft_strlen(argv[2]);
	pid = atoi(argv[1]);
	kill(pid, SIGUSR1);
	send_bits(pid, (void *)&size, sizeof(unsigned long) * 8);
	while (*argv[2])
		send_bits(pid, (void *)argv[2]++, 8);

	return 0;
}