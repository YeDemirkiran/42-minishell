#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/19 20:56:36 by yademirk          #+#    #+#              #
#    Updated: 2025/12/07 20:56:51 by yademirk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = lexer_tester

CC = cc
W_FLAGS = -Wall -Wextra -Werror
CC_FLAGS = $(W_FLAGS) -g

LIB_PATH = ../../lib
LIBFT_PATH = $(LIB_PATH)/libft

LEXER_PATH = ../../src/lexer/
LEXER_SRC = $(addprefix $(LEXER_PATH), lexer.c lexer_utils.c lexer_token.c token_utils.c)

SRC = ./lexer_tester.c $(LEXER_SRC)
INC_FLAGS = -I./ -I../../include/ -I$(LEXER_PATH) -I$(LIBFT_PATH)/include
LIB_FLAGS = -L$(LIBFT_PATH) -lft

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CC_FLAGS) $(INC_FLAGS) $^ $(LIB_FLAGS) -o $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean re
