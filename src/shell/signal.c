#include "../../minishell.h"

void	handle_ctrl_c(int sig, siginfo_t *info, void *context)
{
	int *pipe_fds;
    int data;

	data = 43;
	pipe_fds = (int *)info->si_value.sival_ptr;
    write(pipe_fds[1], &data, sizeof(int));
	printf("Exiting the program.\n");
}

void	handle_ctrl_d(int sig, siginfo_t *info, void *context)
{
	int *pipe_fds;
    int data;

	data = 44;
	pipe_fds = (int *)info->si_value.sival_ptr;
    write(pipe_fds[1], &data, sizeof(int));
	printf("Exiting the shell.\n");
}

// be careful to release the pipes after a failure
// we handle the signal with pipes to ensure no data races are possible
void	catch_the_signal(int *pipe_fds)
{
	struct sigaction	ctrl_c;
	struct sigaction	ctrl_d;
	struct sigaction	ctrl_slash;

	ctrl_c.sa_handler = handle_ctrl_c;
	if (sigemptyset(&ctrl_c.sa_mask) < 0)
		failure("Error initializing signal set.");
	ctrl_c.sa_flags = SA_SIGINFO;
    ctrl_c.sa_value.sival_ptr = pipe_fds;
	if (sigaction(SIGINT, &ctrl_c, NULL) < 0)
		failure("Error initializing signal set.");

	ctrl_d.sa_handler = handle_ctrl_d;
	if (sigemptyset(&ctrl_d.sa_mask) < 0)
		failure("Error initializing signal set.");
	ctrl_d.sa_flags = SA_SIGINFO;
    ctrl_d.sa_value.sival_ptr = pipe_fds;
	if (sigaction(SIGQUIT, &ctrl_d, NULL) < 0)
		failure("Error initializing signal set.");
}