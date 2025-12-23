/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 15:17:08 by muayna            #+#    #+#             */
/*   Updated: 2025/12/23 15:17:08 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
