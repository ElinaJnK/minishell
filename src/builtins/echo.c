#include "minishell.h"

void	failure_exec(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

// on se dit que quand il y a la redirection, on va mettre le fd du fichier de la redirection
int	exec_echo(t_cmd *cmd, int fd_out)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;
	if (!cmd || !cmd->args)
		return (failure_exec("Error: echo doesnt exist"));
	if (cmd->args[0] && !ft_strcmp(cmd->args[0], "-n"))
	{
		n_option = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], fd_out);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', fd_out);
		i++;
	}
	if (!n_option)
		ft_putchar_fd('\n', fd_out);
	return (EXIT_SUCCESS);
}
