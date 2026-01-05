/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 14:26:22 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/05 18:15:00 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_INPUT_H
# define READ_INPUT_H

/**
 * @brief Uses GNU Readline, prepares a multi-line input by the user
 * and returns it.
 */
char	*read_input(const char *primary_prompt, const char *secondary_prompt,
	int interactive);

#endif
