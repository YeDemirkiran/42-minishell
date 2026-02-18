/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:36:55 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:16:14 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_PIPELINE_H
# define PARSE_PIPELINE_H

# include "structs/s_shell.h"

# include "parse_result.h"

/**
 * @brief Parses a pipeline.
 */
t_parse_result	parse_pipeline(t_list *token, t_shell shell);

#endif
