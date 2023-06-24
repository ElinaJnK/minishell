#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../libft/libft.h"

/*----types token----*/
/*
	- 0 : &&
	- 1 : ||
	- 2 : >
	- 3 : *
*/

typedef struct s_token
{
	char			*token;
	int				type;
	struct t_token	*next;
}		t_token;

typedef struct s_abr
{
	char			*str;
	struct t_abr	*g;
	struct t_abr	*d;
}		t_abr;

/*----abr----*/
t_abr	*create_abr(char *expr, int start, int end);

/*----parsing functions----*/
void	read_stdin(char *limiter);
#endif