#include "minishell.h"

void	failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}
/*
void	tchitat()
{
	char	*line;

	line = readline("> ");
	while (line && ft_strncmp(line, limiter, ft_max(ft_strlen(line) - 1,
			ft_strlen(limiter)) != 0)
	{
		add_history(line);
		
	}
}

int	main(int ac, char **av, char **env)
{
	if (!*env)
		return (failure("No environment"), 1);
	
}*/
// // on peut reprendre cette fonction mais on doit faire le parsing sur la ligne
// void	read_stdin(char *limiter)
// {
// 	char	*line;
// 	int		*pipe_fds;
// 	t_abr	*a;
// 	int		ret;

// 	line = readline("> ");
// 	generate_pipes(pipe_fds, 1);
// 	catch_the_signal(pipe_fds);
// 	while (line != NULL && ft_strncmp(line, limiter, ft_max(ft_strlen(line) - 1,
// 				ft_strlen(limiter))) != 0 && shell_running)
// 	{
// 		add_history(line);
// 		a = parsing(line);
// 		//execute(a);
// 		free(line);
// 		line = readline("> ");
// 			// if (read(pipefd[0], &ret, sizeof(int)) == -1)
// 			// 	failure("Error in reading signal output.");
// 			// else
// 			// {
// 			// 	if (ret == 43)
// 			// 		kill();
// 			// 	if (ret == 44)
// 			// 		break ;
// 			// }
// 			// else
// 	}
// 	if (line)
// 		free(line);
// 	close_pipes(pipe_fds, 1);
// }

// int	main(int ac, char **av, char **env)
// {
// 	if (ac > 2)
// 		return (ft_putstr_fd("Error\n", 1), 1);
// 	read_stdin("exit");
// }
