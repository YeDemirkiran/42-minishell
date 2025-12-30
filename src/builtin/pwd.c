/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 15:17:08 by muayna            #+#    #+#             */
/*   Updated: 2025/12/30 22:10:21 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "stdio.h"
#include <libft.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

void pwd(t_envp_list **list)
{
	char buf[1024];

	getcwd(buf, 1024);
	printf("%s\n", buf);
}
