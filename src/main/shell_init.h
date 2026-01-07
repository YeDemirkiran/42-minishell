/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 21:24:27 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/07 21:39:52 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef SHELL_INIT_H
# define SHELL_INIT_H

/**
 * @brief Takes a pointer to a shell struct and initializes it.
 * Argument processing and script file opening also happens here.
 * @return 0 on success, and non-zero exit code on any failure.
 */
int	init_shell(t_shell *shell, int argc, char **argv, char **envp);

#endif
