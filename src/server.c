#include "server.h"

void	set_bit(uint8_t *message, uint8_t bit, int signum)
{
	if (signum == SIGUSR1)
		*message |= (1 << bit);
	else if (signum == SIGUSR2)
		*message &= ~(1 << bit);
}

void	set_int(uint32_t *size, uint8_t bit, int signum)
{
	if (signum == SIGUSR1)
		*size |= (1 << bit);
	else if (signum == SIGUSR2)
		*size &= ~(1 << bit);
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static uint32_t				size;
	static uint8_t				*message;
	static unsigned long long	bit;
	static uint8_t				status;

	(void)context;
	if (status == 0x00)
	{
		if (bit < 32)
		{
			set_int(&size, bit, signum);
			bit++;
		}
		if (bit == 32)
		{
			message = (uint8_t *)malloc(size + 1);
			bit = 0;
			status = 0x01;
		}
		kill(info->si_pid, SIGUSR1);
	}
	else if (status == 0x01)
	{
		if (bit < 8 * size)
		{
			set_bit(&message[bit / 8], bit % 8, signum);
			bit++;
		}
		if (bit == 8 * size)
		{
			message[size] = '\0';
			ft_printf("Received message: %s\n", message);
			free(message);
			bit = 0;
			status = 0x00;
		}
		kill(info->si_pid, SIGUSR1);
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
	while (1)
		pause ();
	return (0);
}
