/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/27 21:15:48 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <structs/s_ast.h>
#include <structs/s_token.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Uses tokens from a lexer and converts them into a full AST
t_ast	*parse(t_token *tokens)
{
	printf("(DEBUG: Starting shell step 2: Parser)\n");
	printf("(DEBUG: Step 1: Parse AST)\n");

	// TODO: Replace this with a helper free_tokens function that frees data inside
	(void)tokens;
	return (NULL);
}
