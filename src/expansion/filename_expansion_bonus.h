/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion_bonus.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:53:23 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:36:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILENAME_EXPANSION_BONUS_H
# define FILENAME_EXPANSION_BONUS_H

# include <structs/s_ast.h>

/**
 * @brief Recursively performs filename expansion on a syntax tree,
 * if the node's type is AST_COMMAND.
 */
int	perform_filename_expansion(t_ast *ast);

#endif
