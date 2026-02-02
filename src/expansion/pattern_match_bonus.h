/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_match_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:48:50 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/02 10:51:31 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PATTERN_MATCH_BONUS_H
# define PATTERN_MATCH_BONUS_H

# include "glob_bonus.h"

/**
 * @brief Takes a pattern and a name, and checks if they do match.
 *
 * @return 1 if pattern matches, 0 otherwise.
 */
int	pattern_match(const char *pattern, const char *name, t_glob_flags flags);

#endif
