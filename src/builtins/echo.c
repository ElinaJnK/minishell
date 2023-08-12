#include "minishell.h"

void	failure_exec(const char *message)
{
	//perror(strerror(errno));
	// if (errno == ENOENT)
	// 	printf("bash: %s: command not found\n", cmd->content);
	perror(message);
	exit(EXIT_FAILURE);
}

// void	write_echo(t_cmd *cmd, int fd_out, int i)
// {
// 	ft_putstr_fd(cmd->args[i], fd_out);
// 	if (cmd->args[i + 1])
// 		ft_putchar_fd(' ', fd_out);
// }
void write_echo(t_cmd *cmd, int fd_out, int i)
{
    char	*arg;
	int		bytes_written;
	
	arg = cmd->args[i]; // Get the argument to be written

    if (arg)
    {
        bytes_written = write(fd_out, arg, ft_strlen(arg));
        if (bytes_written < 0)
        {
            ft_putstr_fd("bash: echo: write error: No space left on device\n", STDERR_FILENO);
            exit(EXIT_FAILURE);
        }
        if (cmd->args[i + 1])
            ft_putchar_fd(' ', fd_out);
    }
}

int	exec_echo(t_cmd *cmd, int fd_out)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;
	if (fd_out < 0)
	{
		ft_putstr_fd("bash: echo: write error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (!cmd || !cmd->args)
		return (ft_putstr_fd("bash: echo: echo not defined\n", fd_out),
			EXIT_FAILURE);
	while (cmd->args[i] && !ft_strncmp(cmd->args[i], "-n", 2))
		i++;
	if (i > 1)
		n_option = 1;
	while (cmd->args[i])
	{
		if (ft_strlen(cmd->args[i]))
			write_echo(cmd, fd_out, i);
		i++;
	}
	if (!n_option)
		ft_putchar_fd('\n', fd_out);
	return (EXIT_SUCCESS);
}
