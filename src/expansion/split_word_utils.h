/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 10:21:05 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/21 10:27:13 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef SPLIT_WORD_UTILS_H
# define SPLIT_WORD_UTILS_H

# include <libft/linked_list.h>

# include "structs/s_shell.h"

char	*get_ifs(t_shell shell);

/**
 * @brief Splits a string into words according to the IFS variable,
 * and returns it in linked list form.
 * @note IFS is retrieved by getenv. If it doesn't exist, then the default
 * IFS is used.
 * @note The defualt IFS is (space)(tab)(newline).
 * @note If IFS is set but has no characters, word splitting is disabled.
 */
t_list	*split_words(char *str, const char *ifs);

int		does_need_split(char *str, const char *ifs);

#endif
