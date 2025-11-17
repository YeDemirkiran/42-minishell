/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/17 15:29:14 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <structs/s_command.h>
#include <enums/e_metachar.h>
#include <types/t_bool.h>
#include <stddef.h>

t_bool	is_blank(char c)
{
	return (c == METACHAR_SPACE || c == METACHAR_TAB);
}

t_command	parse_line(char *line)
{
	unsigned long long	i;
	t_command			command;

	i = 0;
	while (line[i] != '\0')
	{
		if (is_blank(line[i]))
		{
			i++;
			continue ;
		}
		i++;
	}
	return (command);
}
