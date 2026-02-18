/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 11:33:50 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:17:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <unistd.h>
#include <fcntl.h>

// Internal libraries
#include <libft/converter.h>
#include <libft/memory.h>

// Macros
#ifndef GETPID_BUFFER_SIZE
# define GETPID_BUFFER_SIZE 256
#endif

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Returns the shell's own PID by reading the /proc/self/stat file.
 * @return -1 on error, the PID of shell on success.
 * @note This function does not use the getpid() function.
 */
int	get_pid(void)
{
	int		fd;
	int		pid;
	char	buffer[GETPID_BUFFER_SIZE];
	ssize_t	read_size;

	ft_bzero(buffer, GETPID_BUFFER_SIZE);
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	read_size = read(fd, buffer, GETPID_BUFFER_SIZE);
	close(fd);
	if (read_size <= 0)
		return (-1);
	pid = ft_atoi(buffer);
	if (pid <= 0)
		return (-1);
	return (pid);
}

/* ================================== END ================================== */
