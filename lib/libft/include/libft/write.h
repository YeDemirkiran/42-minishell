/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:08:21 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:08:21 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef WRITE_H
# define WRITE_H

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

#endif
