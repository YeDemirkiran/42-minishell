/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:18:57 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdio.h>

// Project headers
#include "structs/s_ast.h"
#include "modules/ast.h"

// Local headers
#include "variable_expansion.h"
#include "tilde_expansion.h"
#include "split_word.h"
#include "remove_quotes.h"

// Bonus headers
#ifdef MINISHELL_BONUS
# include "filename_expansion_bonus.h"
#endif

#ifdef MINISHELL_BONUS

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	if (perform_tilde_expansion(root, shell) != 1)
		return (0);
	if (perform_variable_expansion(root, shell) != 1)
		return (0);
	if (perform_word_split(root, shell) != 1)
		return (0);
	if (perform_filename_expansion(root) != 1)
		return (0);
	perform_quote_removal(root);
	return (1);
}

#else

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	if (perform_tilde_expansion(root, shell) != 1)
		return (0);
	if (perform_variable_expansion(root, shell) != 1)
		return (0);
	if (perform_word_split(root, shell) != 1)
		return (0);
	perform_quote_removal(root);
	return (1);
}

#endif

/* ================================== END ================================== */
