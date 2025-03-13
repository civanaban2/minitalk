/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cari <cari@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:44:01 by cari              #+#    #+#             */
/*   Updated: 2025/03/13 17:35:05 by cari             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft.h"
# include <signal.h>

typedef struct s_package
{
	__uint8_t	status;
	int			size;
	int			index;
	int			bit_count;
	int			pid;
	char		*message;
}	t_package;

#endif