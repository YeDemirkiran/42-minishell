/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/07 22:56:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

#include <modules/builtin.h>
#include <macros/m_colors.h>

#define SHELL_NAME "babashell"

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	char	first_run;

	(void)argc;
	(void)argv;
	(void)envp;
	first_run = 1;
	while (str || first_run)
	{
		if (!first_run)
			free(str);
		first_run = 0;
		str = readline(COLOR_GREEN SHELL_NAME COLOR_WHITE " > ");
		printf("%s\n", str);
	}
	return (EXIT_SUCCESS);
}
