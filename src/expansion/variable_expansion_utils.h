/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:56:07 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:34:56 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_EXPANSION_UTILS_H
# define VARIABLE_EXPANSION_UTILS_H

# include <libft/linked_list.h>

/**
 * @brief Takes a list of expansion data blocks,
 * builds the expanded version of a string and returns it.
 */
char	*get_expanded_word(char *str, t_list *expansion_data);

#endif
