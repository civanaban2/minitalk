#include "server.h"
#include <stdio.h>

void	set_bit(uint8_t *message, uint8_t bit, int signum)
{
	if (signum == SIGUSR1)
		*message |= (1 << bit);
	else if (signum == SIGUSR2)
		*message &= ~(1 << bit);
}

void	set_int(uint64_t *size, uint8_t bit, int signum)
{
	if (signum == SIGUSR1)
		*size |= (1 << bit);
	else if (signum == SIGUSR2)
		*size &= ~(1 << bit);
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static unsigned long long	size;
	static uint8_t				bit;
	static uint8_t				*message;
	static uint8_t				status;
	static uint8_t				*tmp;

	(void)context;


	if (info->si_pid == getpid())
	{
		bit = 0;
		size = 0;
		status = 0x00;
		if (message)
			free(message);
		message = NULL;
		tmp = NULL;
		return ;
	}

	if (status == 0x00)
	{
		if (bit < 64)
		{
			set_int(&size, bit, signum);
			bit++;
		}
		if (bit == 64)
		{
			message = (uint8_t *)malloc(size + 1);
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
		if (kill(info->si_pid, SIGUSR1) == -1)
		{
			ft_printf("Error: kill failed\n");
			exit(1);
		}
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
			ft_printf("Received message: %s\n", message);
			free(message);
			message = NULL;
			tmp = NULL;
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
	int timer;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_printf("Server PID: %d\n", getpid());
	while (1) {
		timer = usleep(1000000);
		if (timer == 0)
		{
			ft_printf("Client doesnt respond, server restarting.\n");
			kill(getpid(), SIGUSR1);
		}
	}
	return (0);
}
