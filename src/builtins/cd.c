#include "minishell.h"

int	cd_without_env(t_cmd *cmd, int output_fd, t_env **lst_env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_putstr_fd("bash: pwd\n", output_fd),
			EXIT_FAILURE);
	if (!in_env("OLDPWD", pwd, *lst_env))
		add_back_env(lst_env,  new_env("OLDPWD", pwd));
	free(pwd);
	if (chdir(cmd->args[1]) == -1)
		return (ft_putstr_fd("bash: cd: HOME not set\n", output_fd),
			EXIT_FAILURE);
	pwd = getcwd(NULL, 0);
	if (!in_env("PWD", pwd, *lst_env))
		add_back_env(lst_env,  new_env("PWD", pwd));
	free(pwd);
	return (EXIT_SUCCESS);
}

int	exec_cd(t_cmd *cmd, int output_fd, t_env **lst_env)
{
	if (!cmd)
		return (ft_putstr_fd("bash: cd\n", output_fd),
			EXIT_FAILURE);
	if (!cmd->args)
		return (EXIT_SUCCESS);
	if (lst_size_env(*lst_env) < 5)
		return (cd_without_env(cmd, output_fd, lst_env));
	else if (chdir(cmd->args[1]) == -1)
		return (ft_putstr_fd("bash: cd\n", output_fd),
			EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
