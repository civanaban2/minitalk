#include "client.h"

int		total = 0;

void	sig_handler(int signum)
{
	if (signum == SIGUSR1)
	{
		total++;
	}
}

void	send_byte(int pid, uint8_t byte)
{
	int i;

	i = 0;
	while (i < 8)
	{
		if (byte & (1 << i))
		{
			kill(pid, SIGUSR1);
			if (usleep(1000000) == 0)
			{
				ft_printf("Server is not responding\n");
			}
		}
		else
		{
			kill(pid, SIGUSR2);
			if (usleep(1000000) == 0)
			{
				ft_printf("Server is not responding\n");
			}
		}
		i++;
	}
}

void send_number(int pid, unsigned long long size)
{
	int i;

	i = 0;
	while (i < 64)
	{
		if (size & (1 << i))
		{
			kill(pid, SIGUSR1);
			if (usleep(1000000) == 0)
			{
				ft_printf("Server is not responding\n");
			}
		}
		else 
		{
			kill(pid, SIGUSR2);
			if (usleep(1000000) == 0)
			{
				ft_printf("Server is not responding\n");
			}
		}
		i++;
	}
}

int main(int argc, char **argv)
{
	unsigned long long	size;
	int 				pid;

	signal(SIGUSR1, sig_handler);
	if (argc != 3)
	{
		ft_printf("Usage: %s <server-pid> <message>\n", argv[0]);
		return 1;
	}
	size = ft_strlen(argv[2]);
	pid = atoi(argv[1]);

	send_number(pid, size);
	while (*argv[2])
	{
		send_byte(pid, *argv[2]);
		argv[2]++;
	}

	return 0;
}