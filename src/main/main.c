/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:06:42 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/07 22:46:03 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

#include <modules/builtin.h>

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	// char	*user_name;
	// char	*cwd;
	char	first_run;

	(void)argc;
	(void)argv;
	(void)envp;
	first_run = 1;
	while (str || first_run)
	{
		if (!first_run)
		{
			free(str);
			// printf("\n");
		}
		first_run = 0;
		// user_name = getenv("USER");
		// cwd = getcwd(NULL, 0);
		//printf("\n(minishell) %s:%s$", user_name, cwd);
		str = readline("minishell > ");
		printf("%s\n", str);
	}
	return (EXIT_SUCCESS);
}
