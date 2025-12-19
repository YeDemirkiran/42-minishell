/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:06:13 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/19 12:16:12 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell_lb.h>

void cd(t_envp_list **list, char *input)
{
    char buf[1024];
    char *old_pwd;
 
    getcwd(buf, 1024);
    update_envp_value(list, "OLDPWD", buf);
    //printf("DEBUG OLDPWD %s\n", find_value_on_envp(list, "OLDPWD"));
    chdir(input);
    getcwd(buf, 1024);
    update_envp_value(list, "PWD", buf);
    //printf("DEBUG PWD %s\n", find_value_on_envp(list, "PWD"));
}
