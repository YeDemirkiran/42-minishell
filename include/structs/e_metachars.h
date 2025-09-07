/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_metachars.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:19:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/07 20:28:19 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef E_METACHARS_H
# define E_METACHARS_H

/**
 * @brief Metacharacters used for the tokenization phase.
 */
enum e_metachars
{
	/** @brief Space character, separates tokens. */
	META_SPACE = ' ',

	/** @brief Tab character, separates tokens. */
	META_TAB = '\t',

	/** @brief Newline, separates commands or lines. */
	META_NL = '\n',

	/** @brief Pipe operator, redirects output to another command. */
	META_PIPE = '|',

	/** @brief Ampersand, for background processes or logical AND. */
	META_AMP = '&',

	/** @brief Semicolon, separates commands on the same line. */
	META_SMCLN = ';',

	/** @brief Left parenthesis, begins subshell or grouping. */
	META_PARENT_LEFT = '(',

	/** @brief Right parenthesis, ends subshell or grouping. */
	META_PAREN_RIGHT = ')',

	/** @brief Less-than, input redirection operator. */
	META_REDIR_LEFT = '<',

	/** @brief Greater-than, output redirection operator. */
	META_REDIR_RIGHT = '>'
};

#endif
