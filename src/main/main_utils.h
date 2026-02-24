/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:12:54 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 14:46:41 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_UTILS_H
# define MAIN_UTILS_H

# include "structs/s_ast.h"

void	init_main(t_shell *shell, int argc, char **argv, char **envp);
void	run_exec(t_ast *ast, t_shell *shell);

#endif