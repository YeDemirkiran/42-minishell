/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:36:55 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/05 18:48:09 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_PIPELINE_H
# define PARSE_PIPELINE_H

# include "parse_result.h"

t_parse_result	parse_pipeline(t_list *token, t_shell shell);

#endif
