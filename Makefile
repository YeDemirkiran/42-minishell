#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/19 12:32:47 by muayna            #+#    #+#              #
#    Updated: 2025/12/23 21:02:38 by yademirk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = minishell

# DIRECTORIES
## Main
SRC_DIR = ./src/
OBJ_DIR = ./obj/
INC_DIR = ./include/

## Libraries
LIB_DIR = ./lib/

LIBMNL_DIR = $(LIB_DIR)minishell_lb/
LIBMNL_INC_DIR = $(LIBMNL_DIR)includes/
LIBMNL_A = $(LIBMNL_DIR)minishell_lb.a

LIBGNL_DIR = $(LIB_DIR)get_next_line/
LIBGNL_INC_DIR = $(LIBGNL_DIR)includes/
LIBGNL_A = $(LIBGNL_DIR)gnl.a

LIBFT_DIR = $(LIB_DIR)libft/
LIBFT_INC_DIR = $(LIBFT_DIR)include/
LIBFT_A = $(LIBFT_DIR)libft.a

LIBGB_DIR = $(LIB_DIR)garbage_collector/
LIBGB_INC_DIR = $(LIBGB_DIR)
LIBGB_A = $(LIBGB_DIR)libgb.a

# C FILES
C_FILES = $(MAIN_FILES) $(LEXER_FILES) $(PARSER_FILES) $(AST_FILES) $(EXECUTOR_FILES) $(BUILTIN_FILES) $(SIGNAL_FILES)

## Main
MAIN_DIR = main/
MAIN_FILES = $(addprefix $(MAIN_DIR), main.c)

## Lexer
LEXER_DIR = lexer/
LEXER_FILES = $(addprefix $(LEXER_DIR), lexer.c lexer_utils.c lexer_misc.c token.c token_utils.c)

## Parser
PARSER_DIR = parser/
PARSER_FILES = $(addprefix $(PARSER_DIR), parser.c parse_command.c parse_redirection.c parse_pipeline.c parse_result.c)

## AST
AST_DIR = ast/
AST_FILES = $(addprefix $(AST_DIR), ast.c ast_traversal.c ast_helper.c)

## Executor
EXECUTOR_DIR = executor/
EXECUTOR_FILES = $(addprefix $(EXECUTOR_DIR), executor.c)

## Built-ins
BUILTIN_DIR = builtin/
BUILTIN_FILES = $(addprefix $(BUILTIN_DIR), cd.c pwd.c env.c)

## Signals
SIGNAL_DIR = signal/
SIGNAL_FILES = $(addprefix $(SIGNAL_DIR), signal.c)

# SOURCES
SRCS = $(addprefix $(SRC_DIR), $(C_FILES))
OBJS = $(addprefix $(OBJ_DIR), $(C_FILES:.c=.o))

# CONFIGURATION
CC = cc
#W_FLAGS = -Wall -Wextra -Werror -g
INC_FLAGS = -I$(INC_DIR) -I$(LIBFT_INC_DIR) -I$(LIBGB_INC_DIR) -I$(LIBGNL_INC_DIR) -I$(LIBMNL_INC_DIR)
LIBS = -L./ -L$(LIBFT_DIR) -L$(LIBGB_DIR) -L$(LIBGNL_DIR) -L$(LIBMNL_DIR)  -lreadline $(LIBMNL_A) $(LIBGNL_A)  -lft -lgb
FLAGS = $(W_FLAGS) $(INC_FLAGS)

# ----------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A) $(LIBGB_A) $(LIBGNL_A) $(LIBMNL_A)
	$(CC) $(FLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT_A):
	make -C $(LIBFT_DIR)

$(LIBGB_A):
	make -C $(LIBGB_DIR)

$(LIBGNL_A): $(LIBFT_A)
	make -C $(LIBGNL_DIR) CFLAGS="-Wall -Wextra -Werror -I../../lib/libft/include -Iincludes"

$(LIBMNL_A): $(LIBFT_A) $(LIBGB_A)
	make -C $(LIBMNL_DIR) CFLAGS="-Wall -Wextra -Werror -I../../lib/libft/include -Iincludes -I../garbage_collector -I../get_next_line/includes"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $^ -c -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)
	make -C $(LIBMNL_DIR) fclean
	make -C $(LIBGB_DIR) fclean
	make -C $(LIBGNL_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
