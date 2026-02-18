/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 11:39:24 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:17:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_PID_H
# define GET_PID_H

/**
 * @brief Returns the shell's own PID by reading the /proc/self/stat file.
 * @return -1 on error, the PID of shell on success.
 * @note This function does not use the getpid() function.
 */
int	get_pid(void);

#endif