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

void	pwd(void)
{
	char	buf[1024];

	getcwd(buf, 1024);
	ft_putendl_fd(buf, STDOUT_FILENO);
}
