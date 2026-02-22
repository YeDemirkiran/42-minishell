/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 12:01:55 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 12:02:02 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_INPUT_H
# define READ_INPUT_H

# include "structs/s_shell.h"

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
			int interactive, t_shell *shell);

#endif
