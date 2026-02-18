/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:53:27 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:34:50 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLIT_WORD_H
# define SPLIT_WORD_H

# include "structs/s_ast.h"

/**
 * @brief Takes a syntax tree and recursively splits words in their arguments.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	perform_word_split(t_ast *ast, t_shell shell);

#endif
