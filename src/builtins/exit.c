#include "minishell.h"

int	*exit_status(void)
{
	static int	exit_s = 0;

	return (&exit_s);
}

int	is_num(char *str)
{
	int	i;

	i = 0;
	if (*str == '-')
		i++;
	while (str[i] == ' ')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			break ;
		i++;
	}
	while (str[i])
	{
		if (str[i] != ' ')
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	do_end(t_all *all, int fd_out)
{
	if (all->n_pipes == 0)
	{
		ft_putstr_fd("\001\033[38;5;217m\002Exiting our minishell, ", fd_out);
		ft_putstr_fd("bye bye !ﾟ･:*｡(ꈍᴗꈍ)ε｀*)~｡*:･ﾟ\n\001\033[0m\002", fd_out);
	}
	free_all(all);
	exit(*exit_status());
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
		if (cmd->nb_args >= 2)
		{
			*exit_status() = EXIT_FAILURE;
			ft_putstr_fd("bash: exit: too many arguments\n", fd_out);
			return (EXIT_FAILURE);
		}
		if (is_num(cmd->args[1]) == EXIT_FAILURE)
		{
			*exit_status() = EXIT_FAILURE;
			ft_putstr_fd("bash: exit: numeric argument required\n", fd_out);
			return (EXIT_FAILURE);
		}
	}
	do_end(all, fd_out);
	return (EXIT_SUCCESS);
}
