/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:57:21 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 21:25:44 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <unistd.h>
#include <stdlib.h>

// Internal libraries
#include <libft/string.h>
#include <libft/write.h>

/* =============================== FUNCTIONS =============================== */

static void	print_version_message(void)
{
	ft_putstr_fd("42 Minishell, Version 1.0 (Jan 2026)\n", STDERR_FILENO);
	ft_putstr_fd("\nAuthors: \n", STDERR_FILENO);
	ft_putstr_fd("- Muhammet Emin Ayna (muayna) "
		"(muayna@student.42istanbul.com.tr)\n", STDERR_FILENO);
	ft_putstr_fd("- Yaşam Ensar Demirkıran (yademirk) "
		"(yademirk@student.42istanbul.com.tr)\n", STDERR_FILENO);
}

static void	print_help_message(void)
{
	ft_putstr_fd("Usage:	minishell ...\n"
		"	minishell script-file ...\n"
		"	minishell option\n"
		"Options:\n"
		"	-h, --help: Displays this message\n"
		"	-v, --version, --info: "
		"Displays version and author info\n", STDERR_FILENO);
}

/**
 * @brief
 * @return 1 for continue, 0 for exit.
 * @note 0 doesn't necessarily means there is an error,
 * it just means you should exit early.
 */
int	process_args(const char **args, int argc)
{
	const char	*first;

	if (argc == 1)
		return (1);
	first = args[1];
	if (first[0] == '-')
	{
		if (ft_strncmp(first, "-v", 2) == 0
			|| ft_strncmp(first, "--version", 9) == 0
			|| ft_strncmp(first, "--info", 6) == 0)
		{
			print_version_message();
			return (0);
		}
		if (ft_strncmp(first, "-h", 2) == 0
			|| ft_strncmp(first, "--help", 6) == 0)
		{
			print_help_message();
			return (0);
		}
	}
	return (1);
}

/* ================================== END ================================== */
