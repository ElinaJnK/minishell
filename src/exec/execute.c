#include <minishell.h>

int    *exit_status(void)
{
    static int    exit_s = 0;

    return (&exit_s);
}

void	exec_and(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	int	status;

	status = 0;
	exec_ast(root->left, input_fd, output_fd, all);
	wait(&status);
	*exit_status() = WEXITSTATUS(status);
	if (WEXITSTATUS(status) == 0)
		exec_ast(root->right, input_fd, output_fd, all);
}

void	exec_or(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	int	status;

	status = 0;
	exec_ast(root->left, input_fd, output_fd, all);
	wait(&status);
	*exit_status() = WEXITSTATUS(status);
	if (WEXITSTATUS(status) != 0)
		exec_ast(root->right, input_fd, output_fd, all);
}

void	exec_pipe(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	int	pipe_fds[2];
	int	status;

	status = 0;
	if (pipe(pipe_fds) < 0)
		failure_exec("fork error");
	exec_ast(root->left, input_fd, pipe_fds[1], all);
	close(pipe_fds[1]);
	wait(&status);
	*exit_status() = WEXITSTATUS(status);
	exec_ast(root->right, pipe_fds[0], output_fd, all);
}

void	exec_redir(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	if (root->left)
	{
		if (root->cmd->input != STDIN_FILENO && root->cmd->output
			!= STDOUT_FILENO)
		{
			exec_ast(root->left, root->cmd->input, root->cmd->output, all);
			// close(root->cmd->input);
			// close(root->cmd->output);
		}
		else if (root->cmd->input != STDIN_FILENO)
		{
			exec_ast(root->left, root->cmd->input, output_fd, all);
			// close(root->cmd->input);
		}
		else if (root->cmd->output != STDOUT_FILENO)
		{
			exec_ast(root->left, input_fd, root->cmd->output, all);
			// close(root->cmd->output);
		}
	}
}

void	exec_ast(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	if (root == NULL)
		return ;
	if (root->left == NULL && root->right == NULL
		&& !(root->cmd->type >= REDIR && root->cmd->type <= DREDIR2_E))
	{
		exec_com(root, input_fd, output_fd, &all);
		return ;
	}
	if (root->cmd->type == AND)
		exec_and(root, input_fd, output_fd, all);
	else if (root->cmd->type == OR)
		exec_or(root, input_fd, output_fd, all);
	else if (root->cmd->type == PIPE)
		exec_pipe(root, input_fd, output_fd, all);
	else if (root->cmd->type >= REDIR && root->cmd->type <= DREDIR2_E)
		exec_redir(root, input_fd, output_fd, all);
}

// void printSpaces(int count)
// {
// 	for (int i = 0; i < count; i++)
// 		printf(" ");
// }

// void printASTHelper(t_ast *node, int depth, int isRight)
// {
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
// 	while (node->cmd->args && i < node->cmd->nb_args + 1)
// 	{
// 		printf("%s ", node->cmd->args[i]);
// 		i++;
// 	}
// 	printf("\n");
// 	printASTHelper(node->left, depth, 0);
// }

// void printAST(t_ast *root)
// {
// 	printf("root: %s\n", root->cmd->content);
// 	printASTHelper(root, 0, 0);
// }

// int main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	// char command[] = "echo a && echo b | echo c";
// 	// char command[] = "echo a | echo c >> f | echo m > f2 | cat ll";
// 	// char command[] = "cat << EOF << hello << bye > f << EOF > g";
// 	// char command[] = "(echo a && echo b) && (echo d && ((echo bruh  > 3 && echo rawr) && echo r)) >> f > c | cat";
// 	// char command[] = "(echo a v && echo \"hello world\") && (echo bruh && echo rawr) | cat";
// 	// char command[] = "cat << EOF << hello << bye > f > g";
// 	//char *command = ft_strdup("cat << \'$USER\' ");
// 	char *command = ft_strdup("echo lolo$?haha");
// 	// command = ft_strdup("ls | grep f > fichier | wc -l > f << EOF | cat");
// 	// command = ft_strdup("ls << EOF > f >> gateau | (echo a && echo b) | wc -l");
// 	// command = ft_strdup("< Makefile cat > /dev/stdout | ls");
// 	// A GERER
// 	// command = ft_strdup("(echo \"&&  ||  |\") && (echo \"&&  ||  |\")");
// 	//command = ft_strdup("echo \"");
// 	//char *command = ft_strdup("&&");
// 	//char command[] = "echo a > f > g >> a";
// 	// char command[] = "(echo $USER'$user' && echo b)";
// 	int count;
// 	t_env *lst_env = spy_env(env);
// 	t_token *t = tokenize(ft_strdup(command), lst_env);
// 	print_list_tok(t);

// 	t_token	*lst_bise = tokenize_bise(t);
// 	print_list_tok(lst_bise);

// 	t_cmd *tokens = transform_into_tab(lst_bise, &count, lst_env);
// 	int i = 0;
// 	printf("----------------------------------------\n");
// 	while (tokens && i < count)
// 	{
// 		printf("%s\t", tokens[i].content);
// 		i++;
// 	}
// 	printf("\n-----------count : %d--------------------\n", count);
// 	t_border *b = malloc(sizeof(t_border));
// 	int n = 0;
// 	b->start = &n;
// 	b->end = count - 1;
// 	t_ast *root = build_ast(tokens, b);
// 	if (root == NULL)
// 	{
// 		printf("root is NULL\n");
// 		return 1;
// 	}
// 	printAST(root);
// 	printf("\n");
// 	exec_ast(root, STDIN_FILENO, STDOUT_FILENO, lst_env);
// 	free_ast(root);
// 	i = 0;
// 	while (i < count)
// 		free(tokens[i++].content);
// 	free(tokens);
// 	return 0;
// }
