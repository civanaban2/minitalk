#include "client.h"

char a = 0b00000000;

void handler(int signum)
{
	if (signum == SIGUSR1)
	{
		a = a << 1 | 0b00000001;
	}
	else if (signum == SIGUSR2)
	{
		a = a << 1;
	}
}

int main(/* int argc, char const *argv[] */)
{
	ft_printf("PID: %d\n", getpid());
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	while (1) {
		ft_printf("%d\n", a);
		ft_printf("%c\n", a);
		sleep(1);
	}
}
