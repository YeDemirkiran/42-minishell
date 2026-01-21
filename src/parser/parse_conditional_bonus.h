/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conditional_bonus.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 21:53:16 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/21 21:54:03 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PARSE_CONDITIONAL_BONUS_H
# define PARSE_CONDITIONAL_BONUS_H

# include <libft/linked_list.h>

# include "structs/s_shell.h"

# include "parse_result.h"

t_parse_result	parse_conditional(t_list *token, t_shell shell);

#endif
