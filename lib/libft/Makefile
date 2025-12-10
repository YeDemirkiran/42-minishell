# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/16 21:19:07 by yademirk          #+#    #+#              #
#    Updated: 2025/12/09 17:57:16 by yademirk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

# ROOT DIRECTORIES
SRC_DIR = src/
INC_DIR = include/
OBJ_DIR = obj/


# SUBDIRECTORIES
## Char
CHAR_DIR = $(SRC_DIR)char/
CHAR_SRC = ft_tolower.c ft_toupper.c
CHAR = $(addprefix $(CHAR_DIR), $(CHAR_SRC))

## Converter
CONVERTER_DIR = $(SRC_DIR)converter/
CONVERTER_SRC = ft_atoi.c ft_itoa.c
CONVERTER = $(addprefix $(CONVERTER_DIR), $(CONVERTER_SRC))

## Linked List
LL_DIR = $(SRC_DIR)linked_list/
LL_SRC = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c ft_lst_to_string_arr.c
LL = $(addprefix $(LL_DIR), $(LL_SRC))

## Math
MATH_DIR = $(SRC_DIR)math/
MATH_SRC = ft_abs.c ft_intlen.c
MATH = $(addprefix $(MATH_DIR), $(MATH_SRC))

## Memory
MEMORY_DIR = $(SRC_DIR)memory/
MEMORY_SRC = ft_bzero.c ft_calloc.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c
MEMORY = $(addprefix $(MEMORY_DIR), $(MEMORY_SRC))

## Predicate
PREDICATE_DIR = $(SRC_DIR)predicate/
PREDICATE_SRC = ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_is_whitespace.c
PREDICATE = $(addprefix $(PREDICATE_DIR), $(PREDICATE_SRC))

## String
STRING_DIR = $(SRC_DIR)string/
STRING_SRC = ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_word_count.c
STRING = $(addprefix $(STRING_DIR), $(STRING_SRC))

## Write
WRITE_DIR = $(SRC_DIR)write/
WRITE_SRC = ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c
WRITE = $(addprefix $(WRITE_DIR), $(WRITE_SRC))

# CONFIGURATION
## Target Files
ALL_SRC = $(CHAR) $(CONVERTER) $(LL) $(MATH) $(MEMORY) $(PREDICATE) $(STRING) $(WRITE)
ALL_OBJ = $(ALL_SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

## Compiler & Flags
CC = cc
FLAGS = -Wall -Wextra -Werror

# ---------------------------------------------------------------

all: $(NAME)
	@echo "Done."

$(NAME): $(ALL_OBJ)
	@echo "Linking object files..."
	@ar rcs $@ $?

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo "Compiling $(notdir $?)"
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $? -o $@ -I$(INC_DIR)

clean:
	@echo "Removing the obj/ folder..."
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Removing the '$(NAME)' file..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
