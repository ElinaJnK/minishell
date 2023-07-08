CC			= cc

NAME		= minishell

# SOURCES_BUILTINS	=				\

# SOURCES_ENV 		=				\

# SOURCES_EXEC		=				\

SOURCES_PARSING 	=	tokenize.c	\

SOURCES_SHELL 		=	main.c		\

SOURCES_UTILS 		=	token.c		\

LIBFT		= libft

CFLAGS		= -Wall -Wextra -Werror -g3

INCLUDE		= include

SRC_DIR		= src

OBJ_DIR		= obj

RM			= rm -f

# OBJS_BUILTINS 	= $(addprefix $(OBJ_DIR)/builtins/,$(SOURCES_BUILTINS:.c=.o))
# OBJS_ENV 		= $(addprefix $(OBJ_DIR)/env/,$(SOURCES_ENV:.c=.o))
# OBJS_EXEC 		= $(addprefix $(OBJ_DIR)/exec/,$(SOURCES_EXEC:.c=.o))
OBJS_PARSING 	= $(addprefix $(OBJ_DIR)/parsing/,$(SOURCES_PARSING:.c=.o))
OBJS_SHELL 		= $(addprefix $(OBJ_DIR)/shell/,$(SOURCES_SHELL:.c=.o))
OBJS_UTILS 		= $(addprefix $(OBJ_DIR)/utils/,$(SOURCES_UTILS:.c=.o))
# OBJS = $(OBJS_BUILTINS) $(OBJS_ENV) $(OBJS_EXEC) $(OBJS_PARSING) $(OBJS_SHELL) $(OBJS_UTILS)
OBJS = $(OBJS_PARSING) $(OBJS_SHELL) $(OBJS_UTILS)
$(info $$SRC is [${OBJS}])
all: lib obj $(NAME)

lib:
	@make -C $(LIBFT)

obj:
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -L $(LIBFT) -lreadline -lft -o $(NAME)

$(OBJ_DIR)/*/%.o: $(SRC_DIR)/*/%.c $(INCLUDE)/minishell.h Makefile
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