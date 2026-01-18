/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_utils.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 00:02:00 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/19 00:05:24 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PARSE_HEREDOC_UTILS_H
# define PARSE_HEREDOC_UTILS_H

/**
 * @brief Generates a random here-document temporary file name at /tmp.
 * @return NULL on failure, a randomized path on success.
 */
char	*get_random_heredoc_path(void);

/**
 * @brief Creates a write-only temporary here-document file at path
 * and returns its file descriptor.
 * @return -1 on error, and an open file descriptor on success.
 */
int		create_temp_heredoc_file(const char *path);

/**
 * @brief Takes a line and compares it with the delimiter.
 */
int		is_delimiter(const char *line, const char *delimiter);

#endif
