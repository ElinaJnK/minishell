#include "../minishell.h"

int	main(int ac, char **av, char **env)
{
	if (ac > 2)
		return (ft_putstr_fd("Error\n", 1), 1);
	read_stdin("exit");
}
