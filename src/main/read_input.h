/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 14:26:22 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/03 14:28:21 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef READ_INPUT_H
# define READ_INPUT_H

/**
 * @brief Uses GNU Readline, prepares a multi-line input by the user
 * and returns it.
 */
char	*read_input(char *primary_prompt, char *secondary_prompt);

#endif
