#ifndef SERVER_H
#define SERVER_H

#include "libft.h"
#include <signal.h>

typedef struct s_package
{
	__uint8_t		status;
	int	size;
	int				pid;
	char			*message;
} t_package;

#endif