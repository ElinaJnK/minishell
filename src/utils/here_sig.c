#include "minishell.h"

int	*exit_here(void)
{
	static int	here = 0;

	return (&here);
}

void	handle_here_c(int sig)
{
	(void)sig;
	close(0);
	*exit_here() = 130;
}

void	signal_here_c(void)
{
	struct sigaction	act;

	act.sa_handler = handle_here_c;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
}
