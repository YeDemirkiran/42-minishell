/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:06:13 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/07 21:32:30 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <stdio.h>

void	change_directory(const char *path)
{
	int	res;

	res = chdir(path);
	if (res != 0)
	{
		// todo: it should write "bash: cd: {path}"
		// todo: return cd's exit status
		// edge case: when path is "", it does nothing
		perror("cd");
	}
}
