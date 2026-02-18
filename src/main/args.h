/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 21:13:33 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:17:36 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H

/**
 * @brief Starts from a given index and shifts arguments to left
 * one time.
 */
void	shift_args_to_left(char **argv, int argc, int start);

/**
 * @brief
 * @return 1 for continue, 0 for exit.
 * @note 0 doesn't necessarily means there is an error,
 * it just means you should exit early.
 */
int		process_args(const char **args, int argc);

#endif
