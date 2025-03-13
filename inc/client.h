/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cari <cari@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:43:58 by cari              #+#    #+#             */
/*   Updated: 2025/03/13 16:43:59 by cari             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft.h"
# include <signal.h>

typedef struct s_package
{
	__uint8_t	status;
	int			size;
	int			pid;
	char		*message;
}	t_package;

#endif