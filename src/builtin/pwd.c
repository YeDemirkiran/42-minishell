/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 01:04:38 by muayna            #+#    #+#             */
/*   Updated: 2026/02/18 01:04:38 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include <libft.h>
#include <stdlib.h>

void	pwd(void)
{
	char	buf[1024];
	char	*check_getcwd;

	check_getcwd = getcwd(NULL, 1024);
	if (check_getcwd == NULL)
		ft_putendl_fd("Zombie Directory", STDOUT_FILENO);
	else
	{
		free(check_getcwd);
		getcwd(buf, 1024);
		ft_putendl_fd(buf, STDOUT_FILENO);
	}
}
