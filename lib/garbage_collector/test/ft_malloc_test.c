/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:45:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 02:23:04 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <gb.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	void	*mem_block;
	size_t	count;
	size_t	size;

	if (argc < 2 || argc > 3)
	{
		printf("usage: ft_malloc_test [memblock_count] memblock_size\n");
		exit(EXIT_FAILURE);
	}

	printf("Starting the test for ft_malloc() garbage collector.\n");

	if (argc == 2)
	{
		count = 1;
		size = atol(argv[1]);
	}
	else
	{
		count = atol(argv[1]);
		size = atol(argv[2]);
	}

	printf("Initializing %lu void* memory block of %lu size.\n", count, size);

	for (size_t i = 0; i < count; i++)
	{
		mem_block = ft_malloc(size * sizeof(void *), 0);

		if (mem_block == NULL)
		{
			printf("Memory block %lu is NULL, ft_malloc() failed. Using perror.\n", i);
			perror("ft_malloc()");
			return (EXIT_FAILURE);
		}
		else
			printf("Memory block %lu with size %lu successfully created.\n", i, size);
	}

	printf("Destroying memory blocks...\n");
	ft_malloc(0, 1);
	printf("Memory blocks successfully destroyed.\n");

	return (EXIT_SUCCESS);
}
