/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/21 15:45:59 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <structs/s_command.h>
#include <structs/s_token.h>
#include <stddef.h>

#include <stdio.h>

t_command	*parse(t_token *tokens)
{
	printf("(DEBUG: Starting shell step 2: Parser)\n");
	printf("(DEBUG: Step 1: Parse AST)\n");
	(void)tokens;
	return (NULL);
}
