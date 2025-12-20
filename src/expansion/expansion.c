/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/20 15:02:48 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "structs/s_ast.h"


#include "stdio.h"
/**
 * @brief Takes an AST and expands arguments in it.
 */
void	expansion(t_ast *root, t_list *env)
{
	(void)root;
	(void)env;
	printf("(DEBUG: Starting shell step 3: Expansion)\n");
	printf("(DEBUG: Step 3.1: Variable Substiton)\n");
}
