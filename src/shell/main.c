#include "minishell.h"

void	failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void print_list_env(t_env *lst_env)
{
	t_env *temp;

	temp = lst_env;
	while (temp)
	{
		printf("%s  =   %s", temp->name, temp->value);
		// printf("[%s]->", temp->content);
		temp = temp->next;
		printf("\n");
	}
}

void	tchitat_stdin(t_all **all)
{
	char		*line;
	t_token		*t;
	t_cmd		*cmds;
	t_border	*b;
	int			count;
	t_ast		*root;
	int			i;

	root = NULL;
	t = NULL;
	cmds = NULL;
	b = malloc(sizeof(t_border));
	if (!b)
		failure("Error in malloc allocation");
	//catch_the_signal();
	line = readline("(▼・ᴥ・▼)$ ");
	while (1)
	{
		//add_history(line);
		t = tokenize(line, (*all)->env);
		t = tokenize_bise(t);
		t = tokenize_crise(t);
		if (t && (*all)->env)
			cmds = transform_into_tab(t, &count, (*all)->env);
		if (!cmds)
			failure("Error in parsing");
		i = 0;
		b->start = &i;
		b->end = count - 1;
		root = build_ast(cmds, b);
		if (root && cmds && (*all)->env)
		{
			(*all)->cmd = cmds;
			(*all)->ast = root;
			(*all)->count = count;
			exec_ast((*all)->ast, STDIN_FILENO, STDOUT_FILENO, *all);
		}
		if (root)
			free_ast(root);
		if (cmds)
			free_cmds(cmds, count);
		if (line)
			free(line);
		root = NULL;
		t = NULL;
		cmds = NULL;
		// rl_on_new_line();
		line = readline("(▼・ᴥ・▼)$ ");
	}
	if (root)
		free_ast(root);
	if (cmds)
		free_cmds(cmds, count);
	if (line)
		free(line);
	if (b)
		free(b);
}

int	main(int ac, char **av, char **env)
{
	t_env	*lst_env;
	t_all	*all;

	(void)ac;
	(void)av;
	if (ac != 1 || av[1])
	{
		printf("This program does not accept arguments\n");
		exit(0);
	}
	if (!*env)
		return (failure("No environment"), 1);
	lst_env = spy_env(env);
	all = build_all(NULL, NULL, lst_env, 0);
	tchitat_stdin(&all);
	if (lst_env)
		free_lst_env(&lst_env);
	//rl_clear_history();
	return (*exit_status());
}

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

/*int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	// char command[] = "echo a && echo b | echo c";
	// char command[] = "echo a | echo c >> f | echo m > f2 | cat ll";
	// char command[] = "cat << EOF << hello << bye > f << EOF > g";
	// char command[] = "(echo a && echo b) && (echo d && ((echo bruh  > 3 && echo rawr) && echo r)) >> f > c | cat";
	// char command[] = "(echo a v && echo \"hello world\") && (echo bruh && echo rawr) | cat";
	// char command[] = "cat << EOF << hello << bye > f > g";
	//char *command = ft_strdup("cat << \'$USER\' ");
	char *command = ft_strdup("(echo a && echo b && echo d > f1) > f2 > f3 >> f4");
	command = ft_strdup("ls | grep f > fichier | wc -l > f << EOF | cat");
	command = ft_strdup("ls << EOF > f >> gateau | (echo a && echo b) | wc -l");
	command = ft_strdup("< Makefile cat > /dev/stdout | ls");
	// A GERER
	command = ft_strdup("(echo \"&&  ||  |\") && (echo \"&&  ||  |\")");
	//command = ft_strdup("echo \"");
	//char *command = ft_strdup("&&");
	//char command[] = "echo a > f > g >> a";
	// char command[] = "(echo $USER'$user' && echo b)";
	int count;
	t_env *lst_env = spy_env(env);
	t_token *t = tokenize(ft_strdup(command), lst_env);
	print_list_tok(t);

	t_token	*lst_bise = tokenize_bise(t);
	print_list_tok(lst_bise);

	t_cmd *tokens = transform_into_tab(lst_bise, &count, lst_env);
	int i = 0;
	printf("----------------------------------------\n");
	while (tokens && i < count)
	{
		printf("%s\t", tokens[i].content);
		i++;
	}
	printf("\n-----------count : %d--------------------\n", count);
	t_border *b = malloc(sizeof(t_border));
	int n = 0;
	b->start = &n;
	b->end = count - 1;
	t_ast *root = build_ast(tokens, b);
	if (root == NULL)
	{
		printf("root is NULL\n");
		return 1;
	}
	printAST(root);
	printf("\n");
	exec_ast(root, STDIN_FILENO, STDOUT_FILENO, lst_env);
	free_ast(root);
	i = 0;
	while (i < count)
		free(tokens[i++].content);
	free(tokens);
	return 0;
}*/
