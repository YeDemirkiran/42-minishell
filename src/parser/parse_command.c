/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/11 17:29:39 by yademirk         ###   ########.fr       */
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
	//printf("(returning: %i)\n", redir);
	return (redir);
}

// Takes a token and saves it in redirections linked list if it's
// a redirection token.
static t_parse_result	parse_redirection(t_list *token, t_list **redirs)
{
	t_parse_result	result;
	t_redir_type	type;
	t_token			*token_obj;
	t_redirection	*redirection;
	t_list			*redir_node;

	token_obj = ((t_token *)token->content);
	// result.current_token = token_obj;
	// if (token->next == NULL || ((t_token *)token->next)->type != TOKEN_WORD)
	// {
	// 	result.current_token = NULL;
	// 	return ((t_parse_result){.type = PARSE_RESULT_TOKEN_ERROR});
	// }
	redirection = malloc(sizeof(t_redirection));
	if (redirection == NULL)
		return ((t_parse_result){.type = PARSE_RESULT_RUNTIME_ERROR});
	type = get_redir_type(token_obj->type);
	if (token_obj->data.number == -1)
		redirection->target_fd = redir_default_fd(type);
	else
		redirection->target_fd = token_obj->data.number;
	redirection->redir_type = type;
	redir_node = ft_lstnew((void *)redirection);
	token_obj = (t_token *)token->next;
	redirection->path = ft_strdup(token_obj->data.word);
	ft_lstadd_back(redirs, redir_node);
	result.type = PARSE_RESULT_SUCCESS;
	result.current_token = token->next->next;
	return (result);
}

// Parses a token according to COMMAND grammar.
// TODO: Remove comments.
static t_parse_result	parse_simple_command(t_list *tokens)
{
	t_parse_result	result;
	t_token			*token_obj;
	t_list			*redirs;
	t_list			*args;

	result.ast = NULL;
	result.current_token = NULL;
	result.type = PARSE_RESULT_RUNTIME_ERROR;
	if (tokens == NULL || tokens->content == NULL)
		return (result);
	redirs = NULL;
	args = NULL;
	while (tokens != NULL)
	{
		token_obj = (t_token *)(tokens->content);
		if (get_redir_type(token_obj->type) != REDIR_NONE)
		{
			//printf("\n\nENTERING REDIR PARSING\n\n");
			parse_redirection(tokens, &redirs);

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
		return (result);
	result.ast = ast_new_node(AST_COMMAND, ast_command_data(ft_lst_to_string_arr(args), redirs), NULL, NULL);
	if (result.ast != NULL)
		result.type = PARSE_RESULT_SUCCESS;
	ft_lstclear(&args, NULL);
	return (result);
}

static t_parse_result	parse_subshell(t_list *tokens)
{
	t_parse_result	result;

	(void)tokens;
	result.type = PARSE_RESULT_RUNTIME_ERROR;
	result.ast = NULL;
	result.current_token = NULL;
	return (result);
}

t_parse_result	parse_command(t_list *tokens)
{
	t_parse_result	result;
	t_token			*token;

	token = (t_token *)tokens->content;
	// if (token->type == TOKEN_NEWLINE)
	// 	result = (t_parse_result){.ast = NULL, .current_token = tokens->next, .type = PARSE_RESULT_SUCCESS};
	if (token->type == TOKEN_WORD || get_redir_type(token->type) != REDIR_NONE)
		result = parse_simple_command(tokens);
	else if (token->type == TOKEN_PAREN_LEFT)
		result = parse_subshell(tokens);
	else
		result = (t_parse_result){.ast = NULL, .current_token = tokens, .type = PARSE_RESULT_TOKEN_ERROR};
	return (result);
}
