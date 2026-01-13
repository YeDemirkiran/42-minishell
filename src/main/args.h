/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 21:13:33 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 21:13:48 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef ARGS_H
# define ARGS_H

/**
 * @brief
 * @return 1 for continue, 0 for exit.
 * @note 0 doesn't necessarily means there is an error,
 * it just means you should exit early.
 */
int	process_args(const char **args, int argc);

#endif
