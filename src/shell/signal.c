#include "minishell.h"

// static void handle_sigint_prompt(int sig)
// {
// 	int last_code;

// 	(void)sig;	
// 	last_code = *exit_status();
// 	if (sig == SIGINT)
// 	{
// 		*exit_status() = 130;
// 		close(0);
// 		write(2, "\n", 1);
// 		rl_replace_line("", 1);
// 		rl_on_new_line();
// 		if (last_code == 130)
// 			rl_redisplay();
// 	}
// 	else if (sig == SIGQUIT)
// 	{
// 		*exit_status() = 131;
// 		signal(SIGQUIT, SIG_IGN);
// 	}
// }

// void	signal_heredoc(void)
// {
// 	struct sigaction	act;

// 	act.sa_handler = handle_sigint_heredoc;
// 	sigemptyset(&act.sa_mask);
// 	act.sa_flags = 0;
// 	sigaction(SIGINT, &act, NULL);
// 	sigaction(SIGQUIT, &act, NULL);
// }

static void handle_sigint_prompt(int sig)
{
	int last_code;

	(void)sig;	
	last_code = *exit_status();
	if (sig == SIGINT)
	{
		*exit_status() = 130;
		write(2, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		//if (last_code == 130)
			rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		//printf("heredoc = %d\n", *heredoc());
		if (*heredoc() == 1)
			ft_putstr_fd("bash: warning: here-document delimited by end-of-file\n", 2);
		*exit_status() = 131;
		signal(SIGQUIT, SIG_IGN);
	}
}


void	signal_prompt(void)
{
	struct sigaction	act;

	act.sa_handler = handle_sigint_prompt;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}

void	signal_exec(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
