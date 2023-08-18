#ifndef MINISHELL_H
#define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"
#include <signal.h>
#include <errno.h>
/*---to delete after tests----*/
#include <string.h>
#include <stdio.h>

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
	DREDIR2_E,
	OPEN_PAR,
	CLOSE_PAR,
};

typedef struct s_error
{
	char 	*error;
	int		num;
	struct s_error *next;
}	t_error;

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
	int		input;
	int		output;
	int		n_pipes;
	int		pid;
	int		status;
	t_token	*lst_err;
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

typedef struct s_tokyo
{
	char	*line;
	char	*content;
	int		q_flag;
	int		i;
	int		expansion;
	t_token	*lst_tok;
	t_env	*lst_env;
}	t_tokyo;

typedef struct s_all
{
	int			n_pipes;
	char		*prompt_good;
	char		*prompt_bad;
	t_env		*env;
	t_ast		*ast;
	t_cmd		*cmd;
	t_border	*b;
	int			count;
}	t_all;

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
void	failure_parse(const char *message, t_token *lst_tok, char *line);
void	failure_exec(const char *message);
void	failure_env(const char *message, char **elem);

/*----parsing functions----*/
void	update_tok(t_tokyo **t);
t_token	*tokenize(char *line, t_env *lst_env);
t_token	*tokenize_bise(t_token *tok);
t_token	*tokenize_crise(t_token *tok, t_env *lst_env);
void	check_tok(t_token **lst_tok);

void	free_tokyo(t_tokyo *t);
t_tokyo	*init_param(char *line, t_env *lst_env);
int		is_true_op(char *line, char **content, t_token **lst_tok);
void	meta_tok(t_tokyo **t);

/*----parsing help functions----*/
t_token	*init_tok(t_token *lst_tok);
char	*ft_addchr(char *s1, char c, t_token *lst_tok, char *line);
t_all	*build_all(t_env *lst_env);
t_tokyo	*init_param(char *line, t_env *lst_env);

/*----wildcard----*/
void	process_wild(const char *pattern, const char *path, t_token **tok, int *flag);

/*----ast tree----*/
void	free_ast(t_ast *a);
t_ast	*create_node(t_cmd *cmd);
t_ast	*build_ast(t_cmd *tokens, t_border *b);

void	ast_redir(t_cmd *tokens, t_ast **root, t_ast **current, int start);
void	ast_op(t_cmd *tokens, t_ast **root, t_ast **current, int start);
void	ast_pipe(t_cmd *tokens, t_ast **root, t_ast **current, int start);
void	ast_cmd(t_cmd *tokens, t_ast **root, t_ast **current, int start);

/*---expansion---*/
char	*expansion(t_tokyo *t);
char	*expansionb(t_tokyo **t);
char	*search_var(char *var, int size, t_env *env);
char	*insert_into_line(char *line, char *var, int start, int end);
char	*expansion_here(char *line, int *i, t_env *lst_env, t_token *lst_tok);

/*----in-utils-----*/
t_cmd	*transform_into_tab(t_token *t, int *count, t_env *env);
t_cmd	*cmd_with_pipeuh(t_token *t, int *count, t_env *env);
int		ft_max(int a, int b);

/*----spy-env----*/
void	add_back_env(t_env **lst_env, t_env *new);
t_env	*spy_env(char **env);
char	**get_env(char *data);
t_env	*new_env(char *name, char *value);
char	**env_to_tab(t_env *lst_env);


void	free_lst_env(t_env **lst_env);
void	update_env(t_env *lst_env, char *name, char *value);
void	lst_del_env(t_env **lst_env, char *name);
int		lst_size_env(t_env *lst);
void	free_tab(char **tab);
int		env_error(char **env, int output_fd);

/*----utils----*/
void	generate_pipes(int *pipe_fds, int num_pipes);
void	close_pipes(int *pipe_fds, int num_pipes);
void	add_back_tok(t_token **lst_tok, t_token *new);
int		is_op(char *line);
int		is_fb(char *line);
int		get_type(char *tok);
t_token	*new_token(char *content, int type);
void	free_lst_tok(t_token **lst_tok);
int		is_heredoc(t_token *lst_tok);
t_token	*last_elem(t_token *lst_tok);
void	here_doc_q(char *line, t_token **lst_tok);
void	free_cmds(t_cmd *cmds, int count);
int		init_op(t_cmd *cmd, t_token *t);
int		lst_size_tok(t_token *lst);
void	free_all(t_all *all);
void	put_error_tok(char *message, t_token **lst_tok);
int		in_env(char *name, char *new_val, t_env *lst_env);

/*----execution----*/
char	*get_command_path(char *command, t_env *env);
void	exec_ast(t_ast *root, int input_fd, int output_fd, t_all *all);
void	exec_com(t_ast *node, int input_fd, int output_fd, t_all **all);
int		*exit_status(void);
int		is_paf(char *cmd);

/*---here doc----*/
int		open_here_doc(int *pipe_fds, char *limiter, int type, t_env *env);

/*----builtins----*/
int		do_builtin(t_cmd *cmd, int output_fd, t_all *all);
int		is_builtin(t_cmd *cmd);
int		exec_cd(t_cmd *cmd, int output_fd, t_env **lst_env);
int		exec_echo(t_cmd *cmd, int fd_out);
int		exec_env(t_cmd *cmd, int fd_out, t_env *lst_env);
int		exec_export(t_cmd *cmd, t_env **lst_env, int output_fd);
int		exec_pwd(t_cmd *cmd, int output_fd);
int		exec_unset(t_cmd *cmd, t_env **lst_env);
int		exec_exit(t_cmd *cmd, t_all *all, int ft_out);
int		is_num(char *str);


/*----signal----*/
void	signal_prompt(void);
void	signal_exec(void);
void	sig_child(void);


#endif