/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 08:14:23 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:30:51 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Project headers
#include "enums/e_redir_type.h"
#include "structs/s_redirection.h"
#include "structs/s_token.h"

/* =============================== FUNCTIONS =============================== */

// Returns the default FD value for a redirection.
int	get_redir_default_fd(t_redir_type type)
{
	int	fd;

	fd = 0;
	if (type == REDIR_INPUT || type == REDIR_HEREDOC)
		fd = 0;
	else if (type == REDIR_OUTPUT || type == REDIR_APPEND)
		fd = 1;
	return (fd);
}

t_redir_type	get_redir_type(t_token_type type)
{
	t_redir_type	redir;

	if (type == TOKEN_REDIR_INPUT)
		redir = REDIR_INPUT;
	else if (type == TOKEN_REDIR_OUTPUT)
		redir = REDIR_OUTPUT;
	else if (type == TOKEN_REDIR_HEREDOC)
		redir = REDIR_HEREDOC;
	else if (type == TOKEN_REDIR_APPEND)
		redir = REDIR_APPEND;
	else
		redir = REDIR_NONE;
	return (redir);
}

/* ================================== END ================================== */
