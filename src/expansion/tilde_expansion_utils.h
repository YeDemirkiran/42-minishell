/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion_utils.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 10:04:39 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:34:52 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TILDE_EXPANSION_UTILS_H
# define TILDE_EXPANSION_UTILS_H

# include <libft/linked_list.h>

int	expand_tilde_word_list(t_list *node, const char *home);
int	expand_tilde_redir_list(t_list *node, const char *home);

#endif
