/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 23:57:54 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/19 00:13:40 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

// Internal libraries
#include <libft/memory.h>
#include <libft/string.h>
#include <libft/linked_list.h>

// Project headers
#include "enums/e_redir_type.h"
#include "structs/s_redirection.h"

// Definitions
#ifndef TMP_HEREDOC_NAME
# define TMP_HEREDOC_NAME "minishell-thd"
#endif

#ifndef TMP_HEREDOC_SUFFIX_LEN
# define TMP_HEREDOC_SUFFIX_LEN 10
#endif

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes a linked list of redirections and unlinks the path
 * of HEREDOC redirections.
 */
void	clear_heredoc_tmp_files(t_list *redirs)
{
	t_redirection	*redir;
	int				unlink_result;

	while (redirs != NULL)
	{
		redir = (t_redirection *)(redirs->content);
		if (redir != NULL && redir->redir_type == REDIR_HEREDOC)
		{
			unlink_result = unlink(redir->path);
			if (unlink_result == -1)
				perror("minishell: warning: "
					"here-document temporary file: unlink");
		}
		redirs = redirs->next;
	}
}

/**
 * @brief Generates a random here-document temporary file name at /tmp.
 * @return NULL on failure, a randomized path on success.
 */
char	*get_random_heredoc_path(void)
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

/**
 * @brief Creates a write-only temporary here-document file at path
 * and returns its file descriptor.
 * @return -1 on error, and an open file descriptor on success.
 */
int	create_temp_heredoc_file(const char *path)
{
	int		fd;

	if (path == NULL)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	return (fd);
}

/**
 * @brief Takes a line and compares it with the delimiter.
 */
int	is_delimiter(const char *line, const char *delimiter)
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

/* ================================== END ================================== */
