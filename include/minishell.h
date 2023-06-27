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


/*----abr----*/
t_abr	*create_abr(char *expr, int start, int end);

/*----parsing functions----*/
void	read_stdin(char *limiter);

/*----utils----*/
void	generate_pipes(int *pipe_fds, int num_pipes);
void	close_pipes(int *pipe_fds, int num_pipes);
#endif