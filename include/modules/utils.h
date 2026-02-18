/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 21:32:10 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 16:13:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "structs/s_shell.h"

// QUOTES.H
/**
 * @brief Takes a character and a pointer to a quote character,
 * and sets it according to the value of the C character.
 *
 * - If C is a single quote ('), and the current quote is not a
 * double quote ("), then the current quote is either set to a single quote or
 * the \0 character.
 *
 * - Vice versa for double quotes.
 */
void	set_quote(char c, char *quote_addr);

// ERROR_HANDLING.H
/**
 * @brief Writes an error message to STDERR with maximum 3 columns.
 */
void	error_message(char *shell_name, char *extra_field, char *custom_error);

/**
 * @brief Prints out an error message with perror (or custom error
 * message if present) in this format:
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

// SHELL_UTILS.H

/**
 * @brief Prints an optional exit messsage depending on 'interactive'
 * and exits with status.
 */
void	shell_exit(unsigned char status, t_shell shell);

/**
 * @brief Empty function that does nothing, just returns 0. This is used
 * to trick GNU Readline's rl_event_hook so it stops periodically to check
 * rl_done variable's value and exits.
 * @return 0
 */
int		rl_event(void);

#endif
