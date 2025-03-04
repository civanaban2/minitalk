#include "server.h"


void signal_handler(int signum, siginfo_t *info, void *ptr)
{
	static unsigned int size; 
	static uint8_t bit_count = 0;
	static uint8_t byte = 0;
	static uint8_t status;
	static char *message;
	static unsigned int i;

	if (status = 0x00)
	{
		if (signum == SIGUSR1)
		{
			size |= 0x01 << bit_count;
		}
		bit_count++;
		if (bit_count == 32)
		{
			status = 0x01;
			bit_count = 0;
			message = malloc(size);
		}
		kill(info->si_pid, SIGUSR1);
	}
	else if (status == 0x01)
	{
		if (signum == SIGUSR1)
		{
			byte |= 0x01 << bit_count;
		}
		bit_count++;
		if (bit_count == 8)
		{
			message[i] = byte;
			i++;
			if (i == size)
			{
				status = 0x00;
				i = 0;
				write(1, message, size);
				free(message);
			}
			bit_count = 0;
		}
	}
}

int main()
{
	struct sigaction act;

	act.sa_sigaction = signal_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	while (1)
	{
		pause();
	}
	return (0);
	
}
