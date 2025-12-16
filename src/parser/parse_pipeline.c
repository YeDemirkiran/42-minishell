/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:36:50 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/16 17:38:32 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "parse_command.h"

t_parse_result	parse_pipeline(t_list *token)
{
	t_parse_result	result;

	result = parse_command(token);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	return (result);
}
