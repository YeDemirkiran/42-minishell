/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:35:09 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/03 18:56:49 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef LEXER_H
# define LEXER_H

# include <structs/s_token.h>

t_token	*lexical_analyze(char *string);

#endif
