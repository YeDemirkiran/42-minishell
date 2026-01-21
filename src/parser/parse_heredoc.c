/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:47:59 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/21 07:01:05 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <readline/readline.h>
#include <libft/string.h>
#include <libft/memory.h>

#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"
#include "modules/expansion.h"
#include "g_sig.h"

#include "../main/read_input.h"

#ifndef TMP_HEREDOC_NAME
# define TMP_HEREDOC_NAME "minishell-thd"
#endif

#ifndef TMP_HEREDOC_SUFFIX_LEN
# define TMP_HEREDOC_SUFFIX_LEN 10
#endif

static char	*get_random_heredoc_path(void)
{
	static const char	hex[] = "0123456789abcdef";
	char				suffix[TMP_HEREDOC_SUFFIX_LEN + 1];
	char				*path;
	int					fd;
	ssize_t				read_len;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (NULL);
	ft_bzero((void *)suffix, sizeof(suffix));
	read_len = read(fd, suffix, (TMP_HEREDOC_SUFFIX_LEN & ~1) / 2);
	if (read_len <= 0)
	{
		close(fd);
		return (NULL);
	}
	while (--read_len >= 0)
	{
		suffix[read_len * 2] = hex[(unsigned char)suffix[read_len] >> 4];
		suffix[read_len * 2 + 1] = hex[(unsigned char)suffix[read_len] & 0x0f];
	}
	if (close(fd) < 0)
		return (NULL);
	path = ft_strjoin("/tmp/" TMP_HEREDOC_NAME "-", suffix);
	return (path);
}

static int	create_temp_heredoc_file(const char *path)
{
	int		fd;

	if (path == NULL)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	return (fd);
}

static int	is_delimiter(const char *line, const char *delimiter)
{
	size_t	len;

	if (line == NULL || delimiter == NULL)
		return (0);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	if (len != ft_strlen(delimiter))
		return (0);
	return (ft_strncmp(line, delimiter, len) == 0);
}

/**
 * @brief Takes a delimiter and removes outer-level quotes from it. Does
 * nothing if it doesn't contain any quotes.
 * @return 0 if no removal has been done, 1 if it has been.
 */
static int	delimiter_remove_quotes(char *delimiter)
{
	size_t	i;

	if (delimiter == NULL)
		return (0);
	i = 0;
	while (delimiter[i] != '\0' && delimiter[i] != '\'' && delimiter[i] != '"')
		i++;
	if (delimiter[i] == '\0')
		return (0);
	remove_quotes(delimiter);
	return (1);
}

static int	check_errno_and_print_error(void)
{
	if (errno != 0)
		return (0);
	ft_putstr_fd("minishell: warning: here-document "
		"delimited by end-of-file\n", STDERR_FILENO);
	return (1);
}

static int	expand_delimiter(int should_expand, char **line_ptr, t_shell shell)
{
	if (should_expand)
	{
		expand_and_replace_string(line_ptr, shell);
		if (*line_ptr == NULL && errno != 0)
		{
			perror("minishell: here-document");
			return (0);
		}
	}
	return (1);
}

/**
 * @brief Takes a delimiter and a file descriptor to an already open file,
 * starts reading from STDIN and writes it to the file, and exits on delimiter
 * match, EOF or SIGINT.
 * @return 0 on failure, 1 on success, 2 on negative FD or null DELIMITER.
 */
static int	read_heredoc(char *delimiter, int fd, t_shell shell)
{
	char	*line;
	int		should_expand;

	if (fd < 0 || delimiter == NULL)
		return (2);
	line = NULL;
	should_expand = delimiter_remove_quotes(delimiter) == 0;
	while (1)
	{
		errno = 0;
		line = get_input_line("> ", shell.interactive);
		if (g_sig != 0)
			return (0);
		if (line == NULL)
			return (check_errno_and_print_error());
		if (is_delimiter(line, delimiter) == 1)
			break ;
		if (expand_delimiter(should_expand, &line, shell) != 1)
			return (0);
		ft_putendl_fd(line, fd);
		free(line);
	}
	if (line != NULL)
		free(line);
	return (1);
}

/**
 * @brief Takes a redirection object and performs here-document reading
 * on it if it's of that type.
 * @return 0 on failure, 1 on success, 2 if REDIR is null,
 * it's PATH is empty or is not a heredoc.
 */
int	parse_heredoc(t_redirection *redir, t_shell shell)
{
	char	*hd_path;
	int		fd;
	int		result;

	if (!redir || redir->redir_type != REDIR_HEREDOC || redir->path == NULL)
		return (2);
	hd_path = get_random_heredoc_path();
	if (hd_path == NULL)
		return (0);
	fd = create_temp_heredoc_file(hd_path);
	if (fd < 0)
	{
		free(hd_path);
		return (0);
	}
	result = read_heredoc(redir->path, fd, shell);
	close(fd);
	free(redir->path);
	redir->path = hd_path;
	if (result == 0)
		return (0);
	return (1);
}
