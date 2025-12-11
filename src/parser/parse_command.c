/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/10 22:15:27 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <libft/string.h>
#include "enums/e_redir_type.h"


#include "structs/s_ast.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"

#include "modules/ast.h"
#include "parse_result.h"

// Returns the default FD value for a redirection.
static int	redir_default_fd(t_redir_type type)
{
	int	fd;

	fd = -1;
	if (type == REDIR_INPUT || type == REDIR_HEREDOC)
		fd = 0;
	else if (type == REDIR_APPEND || type == REDIR_APPEND)
		fd = 1;
	return (fd);
}

static t_redir_type	get_redir_type(t_token_type type)
{
	int	redir;

	redir = -1;
	if (type == TOKEN_REDIR_INPUT)
		redir = REDIR_INPUT;
	else if (type == TOKEN_REDIR_OUTPUT)
		redir = REDIR_OUTPUT;
	else if (type == TOKEN_REDIR_HEREDOC)
		redir = REDIR_HEREDOC;
	else if (type == TOKEN_REDIR_APPEND)
		redir = REDIR_APPEND;
	printf("(returning: %i)\n", redir);
	return (redir);
}


// Parses a token according to COMMAND grammar.
// TODO: Remove comments.
static t_ast	*parse_simple_command(t_list *tokens)
{
	t_ast			*command;
	t_token			*token_obj;
	t_list			*redirs;
	t_list			*current_redir;
	t_list			*args;
	t_redirection	*redirection;
	t_redir_type	redir_type;
	int				skip_next;

	if (tokens == NULL || tokens->content == NULL)
		return (NULL);
	skip_next = 0;
	current_redir = NULL;
	redirs = NULL;
	args = NULL;
	while (tokens != NULL)
	{
		token_obj = (t_token *)(tokens->content);
		if (skip_next == 1)
		{
			skip_next = 0;
			if (token_obj->type == TOKEN_WORD && current_redir != NULL)
				((t_redirection *)(current_redir->content))->path = ft_strdup(token_obj->data.word);
			//else, throw error
			tokens = tokens->next;
			continue ;
		}
		redir_type = get_redir_type(token_obj->type);
		if (redir_type != (t_redir_type)-1)
		{
			redirection = malloc(sizeof(t_redirection));
			if (token_obj->data.number == -1)
				redirection->target_fd = redir_default_fd(redir_type);
			else
				redirection->target_fd = token_obj->data.number;
			redirection->redir_type = redir_type;
			skip_next = 1;
			current_redir = ft_lstnew((void *)redirection);
			ft_lstadd_back(&redirs, current_redir);
		}
		else if (token_obj->type == TOKEN_WORD)
		{
			ft_lstadd_back(&args, ft_lstnew((void *)ft_strdup(token_obj->data.word)));
		}
		else
			break;
		tokens = tokens->next;
	}
	if (args == NULL)
		return (NULL);
	command = ast_new_node(AST_COMMAND, ast_command_data(ft_lst_to_string_arr(args), redirs), NULL, NULL);
	ft_lstclear(&args, NULL);
	return (command);
}

static t_parse_result	parse_subshell(t_list *tokens)
{
	t_parse_result	result;

	(void)tokens;
	result.type = PARSE_RESULT_RUNTIME_ERROR;
	result.ast = NULL;
	result.current_token = tokens;
	return (result);
}

t_parse_result	parse_command(t_list *tokens)
{
	t_parse_result	result;
	t_token			*token;

	token = (t_token *)tokens->content;
	if (token->type == TOKEN_WORD || get_redir_type(token->type) >= 0)
		result = parse_simple_command(tokens);
	else if (result.current_token ->type == TOKEN_PAREN_LEFT)
		result = parse_subshell(tokens);
	else
		result = (t_parse_result){.ast = NULL, .current_token = token, .type = PARSE_RESULT_TOKEN_ERROR};
	return (result);
}
