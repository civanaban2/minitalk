#include "server.h"
#include <stdio.h>

void	set_bit(char *message, char bit, int signum)
{
	if (signum == SIGUSR1)
		*message |= (1 << bit);
	else if (signum == SIGUSR2)
		*message &= ~(1 << bit);
}

void	set_int(unsigned long long *size, char bit, int signum)
{
	if (signum == SIGUSR1)
		*size |= (1 << bit);
	else if (signum == SIGUSR2)
		*size &= ~(1 << bit);
}

int 	set_bits(void *data, int bit_count, int signum)
{
	int i;

	i = 0;
	if (i < bit_count)
	{

	}
}

int	get_length(int bit_count, 

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static t_package		package;
	static char				bit;
	static char				status;
	static char				*tmp;

	(void)context;
	if (status == 0x00)
	{
		if (bit < 64)
		{
			set_int(&size, bit, signum);
			bit++;
		}
		if (bit == 64)
		{
			message = (char *)malloc(size + 1);
			if (!message)
			{
				ft_printf("Error: malloc failed\n");
				exit(1);
			}
			printf("Message size: %llu\n", size);
			tmp = message;
			bit = 0;
			status = 0x01;
		}
		kill(info->si_pid, SIGUSR1);
	}
	else if (status == 0x01)
	{
		if (bit < 8)
		{
			set_bit(tmp, bit, signum);
			bit++;
		}
		if (bit == 8)
		{
			tmp++;
			bit = 0;
			size--;
		}
		if (size == 0)
		{
			*tmp = '\0';
			bit = 0;
			status = 0x00;
			size = 0;
			kill(info->si_pid, SIGUSR1);
			ft_printf("Received message: %s\n", message);
			free(message);
			message = NULL;
			tmp = NULL;
			return ;
		}
		if (kill(info->si_pid, SIGUSR1) == -1)
		{
			ft_printf("Error: kill failed\n");
			exit(1);
		}
	}

}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_printf("Server PID: %d\n", getpid());
	while (1) {
		pause();
	}
	return (0);
}
