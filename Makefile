# Program
NAME := minishell

# Necessities
# -L/usr/lib/x86_64-linux-gnu 

CC := cc
CFLAGS := -Wall -Wextra -Werror -Wpedantic -g3

LIBS := -Llibft -Ilibft/includes -lft -lreadline

SRC :=  main.c list_utils.c \
		builtins_env.c builtins_cd.c \
		builtins_echo.c builtins_pwd.c \
		parser.c parser_utils.c \
		tokenizer.c parse_tokens.c \
		destroy.c print.c 
SRCS := $(addprefix srcs/, $(SRC))

INC := minishell.h
INCLUDES := $(addprefix include/, $(INC))

# Rules

all : $(NAME)

$(NAME) : $(SRCS) $(INCLUDES)
	@make -C libft
	$(CC) $(CFLAGS) -Iinclude $(SRCS) $(LIBS) -o $@

clean :
	@make -C libft clean

fclean :
	@make -C libft fclean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
