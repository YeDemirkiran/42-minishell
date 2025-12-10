#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/16 02:44:01 by yademirk          #+#    #+#              #
#    Updated: 2025/11/16 02:45:05 by yademirk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = ft_malloc_test

C_FILES = ./ft_malloc_test.c
SRC = $(C_FILES)
INC = -I../

LIBGB_DIR_PATH = ../
LIBGB = gb
LIBGB_FULL_PATH = $(LIBGB_DIR_PATH)libgb.a

CC = cc
W_FLAGS = -Wall -Wextra -Werror
LIB_FLAGS = -L$(LIBGB_DIR_PATH) -l$(LIBGB)
CC_ARGS = $(W_FLAGS) $(INC)

# -----------------------------

all: $(NAME)

$(NAME): $(SRC) $(LIBGB_FULL_PATH)
	$(CC) $(CC_ARGS) $(SRC) $(LIB_FLAGS) -o $@

$(LIBGB): $(LIBGB_FULL_PATH)

$(LIBGB_FULL_PATH):
	make -C $(LIBGB_DIR_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
