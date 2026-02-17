# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2026/02/17 17:56:02 by muayna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell
BONUS_NAME = minishell_bonus

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
C_FILES = $(MAIN_FILES) $(LEXER_FILES) $(PARSER_FILES) $(AST_FILES) $(EXPANSION_FILES) $(EXECUTOR_FILES) $(BUILTIN_FILES) $(SIGNAL_FILES) $(UTILS_FILES)

## Main
MAIN_DIR = main/
MAIN_FILES = $(addprefix $(MAIN_DIR), main.c args.c shell_init.c read_input.c read_input_utils.c history.c get_pid.c)

## Lexer
LEXER_DIR = lexer/
LEXER_FILES = $(addprefix $(LEXER_DIR), lexer.c lexer_utils.c lexer_misc.c token.c token_utils.c)

## Parser
PARSER_DIR = parser/
PARSER_FILES = $(addprefix $(PARSER_DIR), parser.c parse_command_list.c parse_command.c parse_simple_command.c parse_redirection.c parse_redirection_utils.c parse_heredoc.c parse_heredoc_utils.c parse_pipeline.c parse_result.c)

## AST
AST_DIR = ast/
AST_FILES = $(addprefix $(AST_DIR), ast.c ast_traversal.c ast_helper.c)

## Expansion
EXPANSION_DIR = expansion/
EXPANSION_FILES = $(addprefix $(EXPANSION_DIR), expansion.c variable_expansion.c variable_expansion_utils.c variable_utils.c expansion_data.c expansion_data_utils.c split_word.c split_word_utils.c remove_quotes.c tilde_expansion.c tilde_expansion_utils.c)

## Executor
EXECUTOR_DIR = executor/
EXECUTOR_FILES = $(addprefix $(EXECUTOR_DIR), executor_fork.c executor_execute_ast.c executor_path.c executor.c executor_errors.c executor_pid_list.c executor_utils.c special_parameter.c shell_level.c)

## Built-ins
BUILTIN_DIR = builtin/
BUILTIN_FILES = $(addprefix $(BUILTIN_DIR), cd.c exit.c cd_error_check.c pwd.c env.c export.c export_utils.c export_utils_2.c export_utils_3.c unset.c echo.c)

## Signals
SIGNAL_DIR = signal/
SIGNAL_FILES = $(addprefix $(SIGNAL_DIR), signal.c)

## Utils
UTILS_DIR = utils/
UTILS_FILES = $(addprefix $(UTILS_DIR), error_handling.c quotes.c shell_utils.c)

## Bonus files
BONUS_FILES = $(addprefix $(PARSER_DIR), parse_conditional_bonus.c parse_subshell_bonus.c) $(addprefix $(EXPANSION_DIR), filename_expansion_bonus.c glob_bonus.c glob_utils_bonus.c pattern_match_bonus.c)

# SOURCES
SRCS = $(addprefix $(SRC_DIR), $(C_FILES))
OBJS = $(addprefix $(OBJ_DIR), $(C_FILES:.c=.o))

BONUS_SRCS = $(addprefix $(SRC_DIR), $(C_FILES) $(BONUS_FILES))
BONUS_OBJS = $(addprefix $(OBJ_DIR), $(C_FILES:.c=.o) $(BONUS_FILES:.c=.o))

# CONFIGURATION
CC = cc
W_FLAGS = -Wall -Wextra -Werror -g
INC_FLAGS = -I$(INC_DIR) -I$(LIBFT_INC_DIR) -I$(LIBGB_INC_DIR) -I$(LIBGNL_INC_DIR) -I$(LIBMNL_INC_DIR)
LIBS = -L./ -L$(LIBFT_DIR) -L$(LIBGB_DIR) -L$(LIBGNL_DIR) -L$(LIBMNL_DIR)  -lreadline $(LIBMNL_A) $(LIBGNL_A)  -lft -lgb
FLAGS = $(W_FLAGS) $(INC_FLAGS)

# ----------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A) $(LIBGB_A) $(LIBGNL_A) $(LIBMNL_A)
	$(CC) $(FLAGS) $(OBJS) $(LIBS) -o $@

bonus: FLAGS += -D MINISHELL_BONUS
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT_A) $(LIBGB_A) $(LIBGNL_A) $(LIBMNL_A)
	$(CC) $(FLAGS) $(BONUS_OBJS) $(LIBS) -o $@

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
	@echo "Starting partial cleanup..."
	@echo "	Removing obj/ dir"
	@rm -rf $(OBJ_DIR)
	@echo "Done. Starting clearing libraries..."

	@echo "	Cleaning Libft:"
	@make -s -C $(LIBFT_DIR) clean
	@echo "	Libft cleared."

	@echo "	Cleaning LibMNL:"
	@make -s -C $(LIBMNL_DIR) clean
	@echo "	LibMNL cleared."

	@echo "	Cleaning LibGB:"
	@make -s -C $(LIBGB_DIR) clean
	@echo "	LibGB cleared."

	@echo "	Cleaning LibGNL:"
	@make -s -C $(LIBGNL_DIR) clean
	@echo "	LibGNL cleared."

	@echo "Partial cleanup is done."

fclean: clean
	@echo "Starting full cleanup..."
	@rm -rf $(NAME)
	@rm -rf $(BONUS_NAME)
	@make -s -C $(LIBFT_DIR) fclean
	@make -s -C $(LIBMNL_DIR) fclean
	@make -s -C $(LIBGB_DIR) fclean
	@make -s -C $(LIBGNL_DIR) fclean

re: fclean all

reb: fclean bonus

.PHONY: all bonus clean fclean re reb
