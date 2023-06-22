CC			= cc

NAME		= minishell

SOURCES =	main.c			\

LIBFT		= libft

CFLAGS		= -Wall -Wextra -Werror -g3

INCLUDE		= include

SRC_DIR		= src

OBJ_DIR		= obj

RM			= rm -f

SRC	=	$(addprefix $(SRC_DIR)/,$(SOURCES))
OBJS =	$(addprefix $(OBJ_DIR)/,$(SOURCES:.c=.o))

all: lib obj $(NAME)

lib:
	@make -C $(LIBFT)

obj:
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -L $(LIBFT) -lreadline -lft -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE)/minishell.h Makefile
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