/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:02:10 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/20 20:08:47 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include <macros/m_colors.h>

void	on_sigint(int i)
{
	(void)i;
	// rl_replace_line("", 0);
	// rl_redisplay();
	// printf("\n"COLOR_GREEN "babashell" COLOR_WHITE " > ");
	// rl_on_new_line();
}
