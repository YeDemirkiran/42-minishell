#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/19 20:56:36 by yademirk          #+#    #+#              #
#    Updated: 2025/12/22 16:36:11 by yademirk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = expansion_tester

CC = cc
W_FLAGS = -Wall -Wextra -Werror
CC_FLAGS = $(W_FLAGS) -g

LIB_PATH = ../../lib
LIBFT_PATH = $(LIB_PATH)/libft
EXP_PATH = ../../src/expansion

SRC = ./expansion_tester.c $(EXP_PATH)/*.c ../../src/ast/*.c
INC_FLAGS = -I./ -I../../include/ -I$(LIBFT_PATH)/include
LIB_FLAGS = -L$(LIBFT_PATH) -lft

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CC_FLAGS) $(INC_FLAGS) $^ $(LIB_FLAGS) -o $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean re
