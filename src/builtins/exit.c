#include "minishell.h"

int	is_num(char *str)
{
	int i;

	i = 0;
	if (*str == '-')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	exec_exit(t_cmd *cmd, t_all *all, int fd_out)
{
	if (!cmd->args)
		return (EXIT_FAILURE);
	if (!cmd->args[1])
		*exit_status() = EXIT_SUCCESS;
	else
	{
		if (is_num(cmd->args[1]) == EXIT_SUCCESS)
		{
			*exit_status() = (ft_atoi(cmd->args[1]) + 255) % 255;
		}
		if (cmd->nb_args >= 2 || is_num(cmd->args[1]) == EXIT_FAILURE)
		{
			*exit_status() = EXIT_FAILURE;
			perror("exit");
			return (EXIT_FAILURE);
		}
	}
	ft_putstr_fd("Exiting our minishell, bye bye ! ﾟ･:*｡(ꈍᴗꈍ)ε｀*)~｡*:･ﾟ", fd_out);
	free_all(all);
	exit(*exit_status());
}