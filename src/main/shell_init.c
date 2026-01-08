/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 21:24:14 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/08 09:53:39 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <readline/readline.h>

#include <libft/memory.h>

#include "modules/utils.h"

#include "get_pid.h"

/**
 * @brief Determines whether the current session is interactive.
 * @return 0 if non-interactive, 1 if interactive, -1 on error.
 */
static int	is_interactive(void)
{
	int	interactive;

	interactive = 0;
	if (isatty(STDIN_FILENO) == 1)
		interactive++;
	else if (errno != ENOTTY)
		return (-1);
	if (isatty(STDERR_FILENO) == 1)
		interactive++;
	else if (errno != ENOTTY)
		return (-1);
	errno = 0;
	return (interactive == 2);
}

/**
 * @brief Determines whether a given script path is valid.
 * @return 0 if the given script file exists, is valid and can be run.
 * Otherwise, returns these values:
 *
 * - 001 - General error
 *
 * - 126 - Access denied, path is a directory, or is a binary file.
 *
 * - 127 - The file doesn't exist
 */
static int	check_script_eligibility(const char *path)
{
	int		fd;
	char	buff[512];
	ssize_t	read_size;
	ssize_t	i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (errno_to_exit_code());
	ft_bzero(buff, 512);
	read_size = read(fd, buff, 512);
	close(fd);
	if (read_size < 0)
		return (errno_to_exit_code());
	i = 0;
	while (i < read_size)
	{
		if (buff[i] == '\0')
			return (126);
		i++;
	}
	return (0);
}

/**
 * @brief Opens the script from argv's second argument (index 1)
 * and redirects it to STDIN via dup2, and modifies the given shell struct
 * according to it.
 * @return 0 on failure, 1 on success.
 */
static int	open_stdin_script(t_shell *shell, int argc, char **argv)
{
	int	fd;
	int	result;

	result = check_script_eligibility(argv[1]);
	if (result != 0)
	{
		if (result == 126 && errno != EISDIR && errno != EACCES)
			shell_error_message(*shell, argv[1], "cannot execute binary file\n");
		else
			shell_error_message(*shell, argv[1], NULL);
		return (result);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		shell_error_message(*shell, argv[1], NULL);
		return (errno_to_exit_code());
	}
	if (fd != STDIN_FILENO)
	{
		result = dup2(fd, STDIN_FILENO);
		close(fd);
		if (result == -1)
		{
			shell_error_message(*shell, argv[1], NULL);
			return (errno_to_exit_code());
		}
	}
	shell->argc = argc - 1;
	shell->args = argv + 1;
	shell->interactive = 0;
	return (0);
}

/**
 * @brief Takes a pointer to a shell struct and initializes it.
 * Argument processing and script file opening also happens here.
 * @return 0 on success, and non-zero exit code on any failure.
 */
int	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	shell->pid = get_pid();
	shell->exit_status = 0;
	shell->argc = argc;
	shell->args = argv;
	shell->env = ft_malloc(sizeof(t_god_struct_envp), 0);
	shell->env->envp_list = create_envp_list(envp);
	shell->env->envp_char = envp_list_to_char_arr(shell->env->envp_list);
	if (argc > 1)
		return (open_stdin_script(shell, argc, argv));
	shell->interactive = is_interactive();
	if (shell->interactive == 1)
		rl_outstream = stderr;
	return (0);
}
