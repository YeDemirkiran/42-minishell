/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 21:32:10 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/07 21:41:57 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef UTILS_H
# define UTILS_H

# include "structs/s_shell.h"

// error_handling.c
/**
 * @brief Prints out an error message with perror (or custom error message if present)
 * in this format:
 *
 * shell_name: [optional_field: ] perror() message
 */
void	shell_error_message(t_shell shell,
	char *extra_field, char *custom_error);

/**
 * @brief Converts the current errno value to shell error codes
 * and returns it. These inclue:
 *
 * - 0 - No error
 *
 * - 1 - Generic error
 *
 * - 126 - Permission denied, Is a directory
 *
 * - 126 - Is a directory
 *
 * - 127 - File or directory not found
 *
 * - 130 - SIGINT
 */
int		errno_to_exit_code(void);

#endif
