/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 21:24:14 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/29 16:28:40 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// Internal libraries
#include <readline/readline.h>
#include <libft/memory.h>

// Project headers
#include "modules/utils.h"

// Local headers
#include "get_pid.h"
#include "args.h"

/* =============================== FUNCTIONS =============================== */

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

static int	on_script_not_eligible(int result, t_shell shell,
	char *file_name)
{
	if (result == 126 && errno != EISDIR && errno != EACCES)
		shell_error_message(shell, file_name, "cannot execute binary file\n");
	else
		shell_error_message(shell, file_name, NULL);
	return (result);
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
		return (on_script_not_eligible(result, *shell, argv[1]));
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		shell_error_message(*shell, argv[1], NULL);
		return (errno_to_exit_code());
	}
	result = dup2(fd, STDIN_FILENO);
	if (fd != STDIN_FILENO)
		close(fd);
	if (result == -1)
	{
		shell_error_message(*shell, argv[1], NULL);
		return (errno_to_exit_code());
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
	shell->env->pid_list = ft_malloc(sizeof(t_pid_list), 0);
	shell->env->pid_list->pid = 0;
	shell->SHLVL += 1;
	if (argc > 1)
	{
		if (ft_strncmp(argv[1], "-s", 2) == 0)
		{
			shift_args_to_left(argv, argc, 1);
			shell->argc -= 1;
		}
		else
			return (open_stdin_script(shell, argc, argv));
	}
	shell->interactive = is_interactive();
	if (shell->interactive == 1)
		rl_outstream = stderr;
	return (0);
}

/* ================================== END ================================== */
