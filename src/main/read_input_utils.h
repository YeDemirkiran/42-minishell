/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 15:30:31 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/03 15:33:56 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef READ_INPUT_UTILS_H
# define READ_INPUT_UTILS_H

/**
 * @brief Checks if an input has unclosed quotes, and returns the type of
 * quote.
 * @return 0 (null) character if input is complete, and the unmatched quote
 * character if it's not.
 */
char	is_input_incomplete(const char *input);

/**
 * @brief Prints an EOF error message to stderr depending on the given
 * quote character.
 */
void	print_uncompleted_eof_error(char quote);

#endif
