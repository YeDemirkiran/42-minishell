/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 14:26:22 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/11 21:44:19 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef READ_INPUT_H
# define READ_INPUT_H

/**
 * @brief Reads and retuns a line from STDIN. Trailing newline is stripped
 * from the line.
 *
 * @return Line on success, NULL on failure.
 *
 * @note Checks if 0 is a TTY and decides between GNU Readline and
 * 42 GNL.
 */
char	*get_input_line(const char *prompt, int interactive);

/**
 * @brief Prepares a multi-line input by the user
 * and returns it.
 * @return Multi-line (or single) input on success, NULL on failure.
 */
char	*read_input(const char *primary_prompt, const char *secondary_prompt,
			int interactive);

#endif
