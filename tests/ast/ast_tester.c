/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tester.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:34:34 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/29 00:35:51 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

typedef struct s_ast_test
{
	char	*name;
	int		major_number;
	int		minor_number;
}				t_parser_test;

typedef struct s_test_result
{
	int	total;
	int	passed;
	int	failed;
}	t_test_result;

void	test_ast(void)
{

}

int	main(void)
{
	printf("\nAST TESTER\n");

	test_ast();
	return (EXIT_SUCCESS);
}
