#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/19 20:56:36 by yademirk          #+#    #+#              #
#    Updated: 2026/01/01 14:12:16 by yademirk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = parser_tester

CC = cc
W_FLAGS = -Wall -Wextra -Werror
CC_FLAGS = $(W_FLAGS) -g

LIB_PATH = ../../lib
LIBFT_PATH = $(LIB_PATH)/libft
GNL_PATH = $(LIB_PATH)/get_next_line
PARSER_PATH = ../../src/parser
EXPANSION_PATH = ../../src/expansion

SRC = ./parser_tester.c $(PARSER_PATH)/*.c $(EXPANSION_PATH)/*.c ../../src/ast/*.c
INC_FLAGS = -I./ -I../../include/ -I$(LIBFT_PATH)/include -I$(GNL_PATH)/includes/
LIB_FLAGS = -L$(LIBFT_PATH) -L$(GNL_PATH) -l:gnl.a -lft

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CC_FLAGS) $(INC_FLAGS) $^ $(LIB_FLAGS) -o $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean re
