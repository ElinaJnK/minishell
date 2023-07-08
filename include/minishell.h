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

typedef struct s_abr
{
	char			*str;
	struct s_abr	*g;
	struct s_abr	*d;
}		t_abr;

typedef struct	s_sig
{
	int				ctrlc;
	int				ctrld;
	int				exit_status;
	pid_t			pid;
}		t_sig;


void	failure(const char *message);

/*----parsing functions----*/
void	read_stdin(char *limiter);

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