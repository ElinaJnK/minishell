#ifndef MINISHELL_H
#define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"
#include <signal.h>
/*---to delete after tests----*/
#include <string.h>

enum e_token
{
	CMD,
	AND,
	OR,
	PIPE,
	REDIR,
	REDIR2,
	DREDIR,
	DREDIR2,
	OPEN_PAR,
	CLOSE_PAR,
};

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	*content;
	char	**args;
	int		nb_args;
	int		type;
}	t_cmd;

typedef struct s_border
{
	int	*start;
	int	end;
}	t_border;

typedef struct s_ast
{
	t_cmd			*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_sig
{
	int		ctrlc;
	int		ctrld;
	int		exit_status;
	pid_t	pid;
}	t_sig;

/*---to delete after tests----*/
void	print_list_tok(t_token *lst_tok);

/*----failure-----*/
void	failure(const char *message);
void	failure_parse(const char *message, t_token *lst_tok);
void	failure_group(const char *message, t_ast *ast);

/*---execution----*/
char	*get_command_path(char *command, t_env *env);

/*----parsing functions----*/
void	read_stdin(char *limiter);
t_token	*tokenize(char *line, t_env *lst_env);
void	check_tok(t_token *lst_tok);

/*----parsing help functions----*/
t_token	*init_param(t_token **lst_tok, int *q_flag, int *i);
void	doo(t_token	**lst_tok, t_token **tok);
t_token	*init_tok(t_token *lst_tok);
char	*ft_addchr(char *s1, char c, t_token *lst_tok);

/*----ast tree----*/
void	free_ast(t_ast *a);
t_ast	*create_node(t_cmd *cmd);
t_ast	*build_ast(t_cmd *tokens, t_border *b);

/*---expansion---*/
char	*expansion(char *line, int *i, t_env *env);

/*----in-utils-----*/
t_cmd	*transform_into_tab(t_token *t, int *count);

/*----spy-env----*/
void	add_back_env(t_env **lst_env, t_env *new);
t_env	*spy_env(char **env);
t_env	*new_env(char *name, char *value);
void	free_lst_env(t_env **lst_env);

/*----utils----*/
void	generate_pipes(int *pipe_fds, int num_pipes);
void	close_pipes(int *pipe_fds, int num_pipes);
void	add_back_tok(t_token **lst_tok, t_token *new);
int		is_op(char *line);
int		is_fb(char *line);
int		get_type(char *tok);
t_token	*new_token(char *content, int type);
void	free_lst_tok(t_token **lst_tok);

/*----signal----*/
void catch_the_signal(int *pipe_fds);
#endif