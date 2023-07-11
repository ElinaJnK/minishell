CC			= cc

NAME		= minishell

# SOURCES_BUILTINS	=				\

# SOURCES_ENV 		=				\

# SOURCES_EXEC		=				\

SOURCES_PARSING 	=	tokenize.c	\
						grouping.c	\
						tcheck.c	\

SOURCES_SHELL 		=	main.c		\

SOURCES_UTILS 		=	token.c		\
						group.c		\

LIBFT		= libft

CFLAGS		= -Wall -Wextra -Werror -g3

INCLUDE		= include

SRC_DIR		= src

OBJ_DIR		= obj

RM			= rm -f

OBJS_PARSING = $(addprefix $(OBJ_DIR)/parsing/,$(SOURCES_PARSING:.c=.o))
OBJS_SHELL = $(addprefix $(OBJ_DIR)/shell/,$(SOURCES_SHELL:.c=.o))
OBJS_UTILS = $(addprefix $(OBJ_DIR)/utils/,$(SOURCES_UTILS:.c=.o))
OBJS = $(OBJS_PARSING) $(OBJS_SHELL) $(OBJS_UTILS)

all: lib obj $(NAME)

lib:
	@make -C $(LIBFT)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -L $(LIBFT) -lreadline -lft -o $(NAME)

obj:
	mkdir -p $(OBJ_DIR) $(OBJ_DIR)/parsing $(OBJ_DIR)/shell $(OBJ_DIR)/utils

$(OBJ_DIR)/parsing/%.o: $(SRC_DIR)/parsing/%.c $(INCLUDE)/minishell.h Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/shell/%.o: $(SRC_DIR)/shell/%.c $(INCLUDE)/minishell.h Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/utils/%.o: $(SRC_DIR)/utils/%.c $(INCLUDE)/minishell.h Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

bonus: $(NAME)

clean:
	make clean -C $(LIBFT)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT)

re: fclean all

.PHONY: all clean fclean re
