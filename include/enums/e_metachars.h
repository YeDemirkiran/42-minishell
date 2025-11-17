/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_metachars.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:19:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/17 12:36:20 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef E_METACHARS_H
# define E_METACHARS_H

// Metacharacters used for the tokenization phase.
typedef enum e_metachars
{
	METACHAR_SPACE = ' ',
	METACHAR_TAB = '\t',
	METACHAR_NEWLINE = '\n',
	METACHAR_PIPE = '|',
	METACHAR_AMPERSAND = '&',
	METACHAR_SEMICOLON = ';',
	METACHAR_PAREN_LEFT = '(',
	METACHAR_PAREN_RIGHT = ')',
	METACHAR_REDIR_LEFT = '<',
	METACHAR_REDIR_RIGHT = '>'
}	t_metachars;

#endif
