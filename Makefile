#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/07 11:06:35 by yademirk          #+#    #+#              #
#    Updated: 2025/09/07 11:18:40 by yademirk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = minishell

# Main directories
INC_DIR = ./include/
SRC_DIR = ./src/
OBJ_DIR = ./obj/

# Compiler
CC = cc

# Flags
W_FLAGS = -Wall -Wextra -Werror
INC_FLAGS = -I$(INC_DIR)
FLAGS = $(W_FLAGS) $(INC_FLAGS)

# C FILES AND DIRECTORIES
C_FILES = $(MAIN_FILES)

#   Main
MAIN_DIR = main/
MAIN_FILES = $(addprefix $(MAIN_DIR), main.c)

# SOURCES
SRCS = $(addprefix $(SRC_DIR), $(C_FILES))
OBJS = $(addprefix $(OBJ_DIR), $(C_FILES:.c=.o))

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $^ -c -o $@

clean:
	rm -rf $(OBJ_DIR)*

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
