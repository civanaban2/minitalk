#include "server.h"
#include <stdio.h>

void comm_init(t_package *package, siginfo_t *info)
{
	package->pid = info->si_pid;
	package->status = 0x01;
}

void get_length(t_package *package, int signum)
{
	static __uint8_t bit_count;

	if (bit_count < sizeof(size_t) * 8)
	{
		if (signum == SIGUSR1){
			package->size |= (1 << bit_count);
			printf("1");
		}
		else if (signum == SIGUSR2){
			package->size &= ~(1 << bit_count);
			printf("0");
		}
		bit_count++;
	}
	if (bit_count == sizeof(size_t) * 8)
	{
		printf("Message size: %lu\n", package->size);
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

void get_message(t_package *package, int signum)
{
	static __uint8_t bit_count;
	static size_t size;

	if (bit_count < sizeof(char) * 8)
	{
		if (signum == SIGUSR1)
			package->message[size] |= (1 << bit_count);
		else if (signum == SIGUSR2)
			package->message[size] &= ~(1 << bit_count);
		bit_count++;
	}
	if (bit_count == sizeof(char) * 8)
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

void signal_handler(int signum, siginfo_t *info, void *context)
{
	static t_package package;

	(void)context;
	if (package.status == 0x00)
	{
		printf("burdayim oc\n");
		comm_init(&package, info);
	}
	else if (package.status == 0x01 && package.pid == info->si_pid)
		get_length(&package, signum);
	else if (package.status == 0x02 && package.pid == info->si_pid)
		get_message(&package, signum);
	kill(info->si_pid, SIGUSR1);
}

int main(void)
{
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);

	ft_printf("Server PID: %d\n", getpid());
	while (1)
		pause();
	return (0);
}