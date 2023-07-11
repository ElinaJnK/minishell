#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"
#include <signal.h>

/*
- 0 : (
- 1 : )
- 2 : &&
- 3 : ||
- 4 : cmd
- 5 : arg (ou p-e mettre cmd + arg ensemble)
- 6 : <
- 7 : >
*/
typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}		t_token;

typedef struct s_ast
{
	t_token			*tok;
	int				type;
	struct s_ast	*left;
	struct s_ast	*right;
}		t_ast;

typedef struct	s_sig
{
	int				ctrlc;
	int				ctrld;
	int				exit_status;
	pid_t			pid;
}		t_sig;

void	print_list_tok(t_token *lst_tok);

/*----parsing functions----*/
void	read_stdin(char *limiter);
t_token	*tokenize(char *line);
void	check_tok(t_token *lst_tok);
void	failure(const char *message);
void	failure_parse(const char *message, t_token *lst_tok);

/*----ast tree----*/
void	free_ast(t_ast *a);
t_ast	*new_node(t_token *tok);
void	failure_group(const char *message, t_ast *ast);

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
void	catch_the_signal(int *pipe_fds);
#endif