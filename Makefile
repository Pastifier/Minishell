# Program
NAME := minishell

# Necessities

CC := cc
CFLAGS := -Wall -Wextra -Werror -Wpedantic -g3 -lreadline

SRC :=  main.c list_utils.c \
		builtins_env.c \
		$(addprefix interpreter/, \
				interpreter.c pipes.c \
				words.c \
		) \
		$(addprefix wrappers/, \
				wrapper_utils.c wrappers.c) \
		builtins_cd.c builtins_echo.c builtins_pwd.c \
		parser.c parser_utils.c \
		tokenizer.c parse_tokens.c \
		destroy.c print.c

SRCS := $(addprefix srcs/, $(SRC))

INC := minishell.h
INCLUDES := $(addprefix includes/, $(INC)) libft/includes/libft.h

# Rules

all : $(NAME)

$(NAME) : $(SRCS) $(INCLUDES)
	@make -C libft
	$(CC) $(CFLAGS) -Iincludes $(SRCS) -Llibft -Ilibft/includes -lft -o $@ -lreadline -lcurses

clean :
	@make -C libft clean

fclean :
	@make -C libft fclean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
