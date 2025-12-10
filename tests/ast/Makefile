#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/19 20:56:36 by yademirk          #+#    #+#              #
#    Updated: 2025/11/29 00:36:18 by yademirk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = ast_tester

CC = cc
W_FLAGS = -Wall -Wextra -Werror
CC_FLAGS = $(W_FLAGS) -g

LIB_PATH = ../../lib
LIBFT_PATH = $(LIB_PATH)/libft
AST_PATH = ../../src/ast

SRC = ./ast_tester.c $(AST_PATH)/ast.c $(AST_PATH)/ast_traversal.c
INC_FLAGS = -I./ -I../../include/ -I$(LIBFT_PATH)/include
LIB_FLAGS = -L$(LIBFT_PATH) -lft

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CC_FLAGS) $(INC_FLAGS) $^ $(LIB_FLAGS) -o $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean re
