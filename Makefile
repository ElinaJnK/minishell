CC			= cc

NAME		= minishell

SOURCES_BUILTINS	=	cd.c		\
						echo.c		\
						env.c		\
						exit.c		\
						export.c	\
						pwd.c		\
						unset.c		\

SOURCES_EXEC		=	com_path.c	\
						exec_ast.c	\
						exec_com.c	\

SOURCES_PARSING 	=	build_ast.c	\
						expansion.c	\
						expansion2.c\
						here_doc.c	\
						spy_env.c	\
						tcheck.c	\
						tok_redir.c	\
						tok_star.c	\
						tok_tik.c	\
						tokenize.c	\
						transf_tab.c\
						wildcard.c	\

SOURCES_SHELL 		=	main.c		\
						signal.c 	\

SOURCES_UTILS 		=	ast_type.c	\
						ast_utils.c	\
						cmds.c		\
						env_utils.c	\
						tok_type.c	\
						tok_utils.c	\

LIBFT		= libft

CFLAGS		= -Wall -Wextra -Werror -g3

INCLUDE		= include

SRC_DIR		= src

OBJ_DIR		= obj

RM			= rm -f

OBJS_PARSING = $(addprefix $(OBJ_DIR)/parsing/,$(SOURCES_PARSING:.c=.o))
OBJS_SHELL = $(addprefix $(OBJ_DIR)/shell/,$(SOURCES_SHELL:.c=.o))
OBJS_UTILS = $(addprefix $(OBJ_DIR)/utils/,$(SOURCES_UTILS:.c=.o))
OBJS_EXEC = $(addprefix $(OBJ_DIR)/exec/,$(SOURCES_EXEC:.c=.o))
OBJS_BUILTINS = $(addprefix $(OBJ_DIR)/builtins/,$(SOURCES_BUILTINS:.c=.o))
OBJS = $(OBJS_PARSING) $(OBJS_SHELL) $(OBJS_UTILS) $(OBJS_EXEC) $(OBJS_BUILTINS)

all: lib obj $(NAME)

lib:
	@make -C $(LIBFT)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -L $(LIBFT) -lreadline -lft -lncurses -o $(NAME)

obj:
	mkdir -p $(OBJ_DIR) $(OBJ_DIR)/parsing $(OBJ_DIR)/shell $(OBJ_DIR)/utils $(OBJ_DIR)/exec $(OBJ_DIR)/builtins

$(OBJ_DIR)/parsing/%.o: $(SRC_DIR)/parsing/%.c $(INCLUDE)/minishell.h Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/shell/%.o: $(SRC_DIR)/shell/%.c $(INCLUDE)/minishell.h Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/utils/%.o: $(SRC_DIR)/utils/%.c $(INCLUDE)/minishell.h Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/exec/%.o: $(SRC_DIR)/exec/%.c $(INCLUDE)/minishell.h Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/builtins/%.o: $(SRC_DIR)/builtins/%.c $(INCLUDE)/minishell.h Makefile
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