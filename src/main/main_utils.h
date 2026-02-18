/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:12:54 by muayna            #+#    #+#             */
/*   Updated: 2026/02/18 15:15:29 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_UTILS_H
# define MAIN_UTILS_H

# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// Internal libraries
# include <gb.h>
# include <get_next_line.h>
# include <minishell_lb.h>
# include <readline/readline.h>

// Project headers
# include "g_sig.h"
# include "macros/m_colors.h"
# include "modules/ast.h"
# include "modules/builtin.h"
# include "modules/executor.h"
# include "modules/expansion.h"
# include "modules/lexer.h"
# include "modules/parser.h"
# include "modules/signal.h"
# include "modules/utils.h"
# include "structs/s_ast.h"
# include "structs/s_shell.h"

// Local headers
# include "../lexer/token.h"
# include "args.h"
# include "history.h"
# include "main_utils.h"
# include "read_input.h"
# include "shell_init.h"

void	init_main(t_shell *shell, int argc, char **argv, char **envp);

#endif