#include <minishell.h>

/*
int	exec_rec(root, args??, impu):
	if (!root->left && !root->right):
		return (execution(root->content))
	if (root->content == &&):
		rep_left = exec_rec(root->left)
		if (rep_left == FAILURE):
			return exit status FAILURE // on finit l'execution
		rep_right = exec_rec(root>right)

	if (root->content == ||):
		rep_left = exec_rec(root->left)
		if (rep_left == SUCCES):
			return exit status SUCCES // on finit l'execution
		rep_right = exec_rec(root>right)

	if (root->content == |):
		rep_left = exec_rec(root->left) // recuperer la valeur renvoyee par le fils gauche et la donner au fils droite
		rep_right = exec_rec(root->right, rep_left)

	if (root->content == > || root->content == < || root->content == >>):
		t_ast *tmp = root->right

		while (tmp->right):
			if (tmp->left): // y a un fichier a creer
				char *f = tmp->content
				OPEN etc et selon <,> ou >> on fait append ou pas
			tmp = tmp->right
		char *fichier = tmp->cmd->content
		int out_fd = OPEN(fichier)
		rep_left = exec_rec(root->left, input_fd, out_fd)
		return rep_left

	if (root->content == <<): //here_doc
		char *delim = root->right
		if (!delim):
			erreur
		char *fichier = faire gnl ou comme on a fait dans pipex
		rep_left = exec_rec(root->left, fichier)
		return rep_left
*/

void	do_execve(int pid, t_cmd *cmd, t_env *env)
{
	char	*path;

	path = get_command_path(com[0], env);
	if (!path)
		failure("path");
	if (execve(path, com, env) < 0)
		failure("execve");
	else if (pid < 0)
		failure("pid");
}

void exec_com(t_ast *node, int input_fd, int output_fd)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		if (node->cmd->nb_args)
		{
			printf("exeeec\n");
			path = get_command_path(node->cmd->content, env);
			if (!path)
				failure("path");
			if (execve(path, com, p->env) < 0)
				failure("execve");
			else if (pid < 0)
				failure("pid");
			execlp(node->cmd->content, node->cmd->content, node->cmd->args, NULL);
		}
		else
		{
			execlp(node->cmd->content, node->cmd->content, NULL);
		}
		perror("exec error");
		exit(1);
	}
	else
		waitpid(pid, &status, 0);
}

void exec_ast(t_ast *root, int input_fd, int output_fd)
{
	int	status;

	status = 0;
	if (root == NULL)
		return ;
	if (root->left == NULL && root->right == NULL)
	{
		exec_com(root, input_fd, output_fd);
		return ;
	}
	if (root->cmd->type == AND)
	{
		exec_ast(root->left, input_fd, output_fd);
		//close(pipe_fds[1]);
		wait(&status);
		if (WEXITSTATUS(status) == 0)
			exec_ast(root->right, input_fd, output_fd);
	}
	else if (root->cmd->type == OR)
	{
		exec_ast(root->left, input_fd, output_fd);
		wait(&status);
		if (WEXITSTATUS(status) != 0)
		{
			exec_ast(root->right, input_fd, output_fd);
		}
	}
	else if (root->cmd->type == PIPE)
	{
		int	pipe_fds[2];
		if (pipe(pipe_fds) < 0)
		{
			perror("pipe error");
			exit(1);
		}
		exec_ast(root->left, input_fd, pipe_fds[1]);
		close(pipe_fds[1]);
		wait(&status);
		exec_ast(root->right, pipe_fds[0], output_fd);
	}
}

// void printSpaces(int count) {
//     for (int i = 0; i < count; i++)
//         printf(" ");
// }

// void printASTHelper(t_ast* node, int depth, int isRight) {
// 	if (node == NULL)
// 		return;

// 	int INDENTATION_SIZE = 4;
// 	depth += INDENTATION_SIZE;
// 	printASTHelper(node->right, depth, 1);
// 	printSpaces(depth - INDENTATION_SIZE);

// 	if (isRight)
// 		printf("┌─");
// 	else
// 	{
// 		printf("└─");
// 	}
// 	printf("%s ", node->cmd->content);
// 	int i = 0;
// 	while (node->cmd->args && i < node->cmd->nb_args)
// 	{
// 		printf("%s ", node->cmd->args[i]);
// 		i++;
// 	}
// 	printf("\n");
// 	printASTHelper(node->left, depth, 0);
// }

// void printAST(t_ast* root) {
// 	printf("root: %s\n", root->cmd->content);
//     printASTHelper(root, 0, 0);
// }

// int main(int argc, char **argv, char **env) {
// 	//char command[] = "cmd1 || ( cmd2 && ( cmd3 || cmd4 ) )";
// 	// char command[] = "( 1 && 2 ) || ( ( 3 || 4 || 5 ) && ( 6 || 7 ) ) && 8";
// 	//char command[] = "( cmd3 || cmd4 ) && cmd5";
// 	//char command[] = "cmd1 || ( cmd3 || cmd4 ) && cmd5";
// 	//char command[] = "( cmd3 || cmd4 ) | cmd5";
// 	//char command[] = "cmd1 > e && cmd2 && cmd3 > d > f3";
// 	//char command[] = "(echo h && echo k) > fichier";
// 	(void)argc;
// 	(void)argv;
// 	printf("%s\n", env[0]);
// 	//char command[] = "echo a > fichier1 && echo c || cat fichier && echo rawr > fichier2 > fichier3";
// 	// char command[] = "echo New line < output.txt";
// 	// char command[] = "echo a > f > g > e";
// 	//char command[] = "echo a > f1 && echo b || cat f1 && echo c > f2 > f3";
// 	//char command[] = "echo c && (echo a && cat << EOF) > f1";
// 	//char command[] = "(echo a && echo b) && (echo d && ((echo bruh  > 3 && echo rawr) && echo r)) >> f > c | cat";
// 	//char command[] = "(echo a && echo b) && (echo bruh && echo rawr) | cat";
// 	//char command[] = "(echo a && echo b) && (echo bruh  > 3 && echo rawr) >> f > c | cat";
// 	//char command[] = "echo a | echo c >> f | echo m > f2 | cat ll";
// 	//char command[] = "echo a > '' > f2 | echo c";
// 	//char command[] = "cmd << LIMITER | cmd1 >> file | echo pipe";
// 	char command[] = "echo a && echo b | echo c";
// 	int count;
// 	t_env *lst_env = spy_env(env);
// 	t_token *t = tokenize(command, lst_env);
// 	print_list_tok(t);
// 	t_cmd *tokens = transform_into_tab(t, &count);
// 	int i = 0;
// 	printf("----------------------------------------\n");
// 	while(tokens && i < count)
// 	{
// 		printf("%s\t", tokens[i].content);
// 		i++;
// 	}
// 	printf("\n-----------count : %d--------------------\n", count);
// 	t_border *b = malloc(sizeof(t_border));
// 	int n = 0;
// 	b->start = &n;
// 	b->end = count - 1;
// 	t_ast* root = build_ast(tokens, b);
// 	printAST(root);
// 	printf("\n");
// 	exec_ast(root, STDIN_FILENO, STDOUT_FILENO);
// 	free_ast(root);
// 	i = 0;
// 	while (i < count)
// 		free(tokens[i++].content);
// 	free(tokens);
// 	return 0;
// }
