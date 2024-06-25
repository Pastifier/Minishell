# Program

NAME := minishell

# Necessities
CC := cc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g3 #-fsanitize=address,undefined
OS := $(shell uname)

Dar = Darwin
Lin = Linux

ifeq (${OS}, ${Dar})
	CFLAGS += -L/Users/ebinjama/homebrew/opt/readline/lib -I/Users/ebinjama/homebrew/opt/readline/include
else ifeq (${OS}, ${Lin})
	CFLAGS += -lreadline -lcurses
else
	$(error Idk, man. Doesn't look like something I was BUILT to deal with ;3)
endif

SRC :=  main.c list_utils.c \
		$(addprefix parsing/, \
				tokenizer.c parser.c parser_utils.c parse_word_utils.c\
				tokenizer_helper.c token_list_utils.c parse_env_spaces.c \
				skip_dummies.c \
		) \
		$(addprefix interpreter/, \
				interpreter.c pipes.c words.c \
				prepare.c redirections.c \
				\
				$(addprefix processor_norme_dump/, \
						interpret_dump.c \
						heredoc_dump.c \
						word_dump.c \
						heredoc_env_expansion.c \
				) \
		) \
		$(addprefix wrappers/, \
				wrapper_utils.c wrappers.c \
				$(addprefix wrapper_norme_dump/, \
						wrapper_dump.c \
				) \
		) \
		$(addprefix builtins/, \
				builtins_cd.c builtins_pwd.c builtins_env.c \
				builtins_echo.c builtins_utils.c \
				$(addprefix builtins_norme_dump/, \
						builtin_dump.c \
				) \
		) \
		destroy.c print.c

#$(addprefix wrappers_norme_dump/,
#		wrapper_dump.c
#)
SRCS := $(addprefix srcs/, $(SRC))

INC := minishell.h parser.h interpreter.h
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
