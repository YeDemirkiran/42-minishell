/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 16:01:01 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/31 19:12:04 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef REMOVE_QUOTES_H
# define REMOVE_QUOTES_H

# include "structs/s_ast.h"

/**
 * @brief Takes a string and removes outer-level quotes from it.
 * @note Quotes inside quotes are preserved.
 */
void	remove_quotes(char *str);
void	perform_quote_removal(t_ast *ast);

#endif
