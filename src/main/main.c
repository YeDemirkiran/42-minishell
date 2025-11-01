/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/20 20:46:55 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>

#include <modules/builtin.h>
#include <modules/signal.h>
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
	signal(SIGINT, on_sigint);
	while (str || first_run)
	{
		if (!first_run)
			free(str);
		first_run = 0;
		str = readline(COLOR_GREEN SHELL_NAME COLOR_WHITE " > ");
		// time to read the line

		// but ignore comments!
		// todo: move this step inside the parsing logic, make it so it starts ignoring the part after the # symbol
		if (str[0] == '#') // ignore comments
			continue;
		if (str && strlen(str) > 0)
			printf("%s\n", str);
	}
	return (EXIT_SUCCESS);
}
