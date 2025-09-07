/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_metachars.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:19:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/07 21:04:38 by yademirk         ###   ########.fr       */
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
	METACHAR_SPACE = ' ',

	/** @brief Tab character, separates tokens. */
	METACHAR_TAB = '\t',

	/** @brief Newline, separates commands or lines. */
	METACHAR_NEWLINE = '\n',

	/** @brief Pipe operator, redirects output to another command. */
	METACHAR_PIPE = '|',

	/** @brief Ampersand, for background processes or logical AND. */
	METACHAR_AMPERSAND = '&',

	/** @brief Semicolon, separates commands on the same line. */
	METACHAR_SEMICOLON = ';',

	/** @brief Left parenthesis, begins subshell or grouping. */
	METACHAR_PAREN_LEFT = '(',

	/** @brief Right parenthesis, ends subshell or grouping. */
	METACHAR_PAREN_RIGHT = ')',

	/** @brief Less-than, input redirection operator. */
	METACHAR_REDIR_LEFT = '<',

	/** @brief Greater-than, output redirection operator. */
	METACHAR_REDIR_RIGHT = '>'
};

#endif
