/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:47:59 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/30 22:38:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <libft/string.h>
#include <libft/memory.h>
#include <get_next_line.h>
#include <minishell_lb.h>

#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"

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

static int	create_temp_heredoc_file(char *path)
{
	int		fd;

	if (path == NULL)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	return (fd);
}

static int	read_heredoc(t_redirection *redir, int fd)
{
	char	*line;

	if (fd < 0 || redir == NULL)
		return (2);
	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		if (line != NULL)
			free(line);
		line = get_next_line(0);
		if (line == NULL)
		{
			if (errno != 0)
				return (0);
			else
			{
				write(2, "\nminishell: warning: here-document delimited by end-of-file\n", 59);
				return (1);
			}
		}
		if (ft_strncmp(line, redir->path, ft_strlen(line) - 1) == 0)
			break ;
		write(fd, line, ft_strlen(line));
	}
	if (line != NULL)
		free(line);
	return (1);
}

int	parse_heredoc(t_redirection *redir)
{
	char	*hd_path;
	int		fd;

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
	if (read_heredoc(redir, fd) == 0)
	{
		free(hd_path);
		close(fd);
		return (0);
	}
	free(redir->path);
	redir->path = hd_path;
	if (close(fd) < 0)
		return (0);
	return (1);
}
