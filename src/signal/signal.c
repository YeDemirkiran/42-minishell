/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:02:10 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/27 18:05:49 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include <macros/m_colors.h>

// Runs on each SIGINT capture
void	on_sigint(int i)
{
	(void)i;
	printf("\n(DEBUG: SIGINT catched.)\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
