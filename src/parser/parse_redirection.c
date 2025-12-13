/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:26:52 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/13 14:43:27 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include <libft/string.h>

#include "enums/e_redir_type.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"

#include "parse_result.h"

// Returns the default FD value for a redirection.
static int	get_redir_default_fd(t_redir_type type)
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

/**
 * @brief Takes a token and saves it in redirections linked list if it's
 * a redirection token.
 */
t_parse_result	parse_redirection(t_list *token, t_list **redirs)
{
	t_redir_type	type;
	t_token			*token_obj;
	t_redirection	*redirection;
	t_list			*redir_node;

	token_obj = ((t_token *)token->content);
	redirection = malloc(sizeof(t_redirection));
	if (redirection == NULL)
		return (parse_runtime_error(token));
	type = get_redir_type(token_obj->type);
	if (token_obj->data.number < 0)
		redirection->target_fd = get_redir_default_fd(type);
	else
		redirection->target_fd = token_obj->data.number;
	redirection->redir_type = type;
	redir_node = ft_lstnew((void *)redirection);
	token_obj = (t_token *)token->next->content;
	redirection->path = ft_strdup(token_obj->data.word);
	ft_lstadd_back(redirs, redir_node);
	return (parse_success(NULL, token->next->next));
}
