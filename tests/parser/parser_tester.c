/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:14:20 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/21 16:22:59 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <libft/string.h>

#include <structs/s_command.h>
#include <structs/s_token.h>
#include <enums/e_token_type.h>
#include <types/t_bool.h>

void	conduct_parser_test(char *test_name, int test_num_major, int test_number_minor, t_token *tokens)
{
	if (tokens == NULL)
		return ;
	
	printf("Test %i.%i: %s\n", test_num_major, test_number_minor, test_name);
}

void	test_parser(int	test_number)
{
	printf("\nTest %i: Testing parser() function\n", test_number);

	conduct_parser_test("ahah", test_number, 1, NULL);
}

int	main(void)
{
	printf("\nPARSER TESTER\n");

	test_parser(1);
	return (EXIT_SUCCESS);
}
