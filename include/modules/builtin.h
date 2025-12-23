/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:24:16 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/23 21:02:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef BUILTIN_H
# define BUILTIN_H

void	cd(t_envp_list **list, t_ast *ast);
void	pwd(t_envp_list **list);
void	env(t_envp_list **list);

#endif
