#include <minishell.h>

void	exec_and(t_ast *root, int input_fd, int output_fd, t_env *lst_env)
{
	int	status;

	status = 0;
	exec_ast(root->left, input_fd, output_fd, lst_env);
	wait(&status);
	if (WEXITSTATUS(status) == 0)
		exec_ast(root->right, input_fd, output_fd, lst_env);
}

void	exec_or(t_ast *root, int input_fd, int output_fd, t_env *lst_env)
{
	int	status;

	status = 0;
	exec_ast(root->left, input_fd, output_fd, lst_env);
	wait(&status);
	if (WEXITSTATUS(status) != 0)
		exec_ast(root->right, input_fd, output_fd, lst_env);
}

void	exec_pipe(t_ast *root, int input_fd, int output_fd, t_env *lst_env)
{
	int	pipe_fds[2];
	int	status;

	status = 0;
	if (pipe(pipe_fds) < 0)
		failure_exec("fork error");
	exec_ast(root->left, input_fd, pipe_fds[1], lst_env);
	close(pipe_fds[1]);
	wait(&status);
	exec_ast(root->right, pipe_fds[0], output_fd, lst_env);
}

void	exec_redir(t_ast *root, int input_fd, int output_fd, t_env *lst_env)
{
	t_ast	*tmp;
	int		fd;

	fd = 1;
	tmp = root->right;
	(void)output_fd;
	while (tmp->right)
	{
		if (tmp->left)
			open(tmp->left->cmd->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		tmp = tmp->right;
	}
	if (tmp->cmd->type == DREDIR)
		fd = open(tmp->cmd->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(tmp->cmd->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	exec_ast(root->left, input_fd, fd, lst_env);
}

void	lims_rec(t_ast *root, t_token **lst_lim)
{
	if (!root)
		return ;
	if (root->left == NULL | root->right == NULL)
		add_back_tok(lst_lim, new_token(root->cmd->content, 0));
	// else if (root->cmd->type != DREDIR2)
	// 	return ;
	else
	{
		lims_rec(root->left, lst_lim);
		lims_rec(root->right, lst_lim);
	}
}

void	exec_here(t_ast *root, int input_fd, int output_fd, t_env *lst_env)
{
	int		pipe_fds1[2];
	int		pipe_fds2[2];
	t_token	*lst_lim;
	t_token	*tmp;

	(void)input_fd;
	lst_lim = NULL;
	if (root->right->cmd->type != DREDIR2)
		lims_rec(root->right->left, &lst_lim);
	else
		lims_rec(root->right, &lst_lim);
	tmp = lst_lim;
	print_list_tok(lst_lim);
	if (!lst_lim)
		failure("pas de limiteur");
	while (tmp)
	{
		open_here_doc(pipe_fds1, tmp->content);
		tmp = tmp->next;
	}
	//if (lst_lim)
	//	free_lst_tok(&lst_lim);
	//exec_ast(root->left, pipe_fds[0], output_fd, lst_env);
	if (pipe(pipe_fds2) < 0)
		failure("pipe");
	exec_ast(root->left, pipe_fds1[0], pipe_fds2[1], lst_env);
	exec_ast(root->right, pipe_fds2[0], output_fd, lst_env);
}

void	exec_ast(t_ast *root, int input_fd, int output_fd, t_env *lst_env)
{
	if (root == NULL)
		return ;
	if (root->left == NULL && root->right == NULL)
	{
		exec_com(root, input_fd, output_fd, lst_env);
		return ;
	}
	if (root->cmd->type == AND)
		exec_and(root, input_fd, output_fd, lst_env);
	else if (root->cmd->type == OR)
		exec_or(root, input_fd, output_fd, lst_env);
	else if (root->cmd->type == PIPE)
		exec_pipe(root, input_fd, output_fd, lst_env);
	else if (root->cmd->type == REDIR || root->cmd->type == REDIR2
		|| root->cmd->type == DREDIR)
		exec_redir(root, input_fd, output_fd, lst_env);
	else if (root->cmd->type == DREDIR2)
		exec_here(root, input_fd, output_fd, lst_env);
}

void printSpaces(int count) {
    for (int i = 0; i < count; i++)
        printf(" ");
}

void printASTHelper(t_ast* node, int depth, int isRight) {
	if (node == NULL)
		return;

	int INDENTATION_SIZE = 4;
	depth += INDENTATION_SIZE;
	printASTHelper(node->right, depth, 1);
	printSpaces(depth - INDENTATION_SIZE);

	if (isRight)
		printf("┌─");
	else
	{
		printf("└─");
	}
	printf("%s ", node->cmd->content);
	int i = 0;
	while (node->cmd->args && i < node->cmd->nb_args + 1)
	{
		printf("%s ", node->cmd->args[i]);
		i++;
	}
	printf("\n");
	printASTHelper(node->left, depth, 0);
}

void printAST(t_ast* root) {
	printf("root: %s\n", root->cmd->content);
    printASTHelper(root, 0, 0);
}

int main(int argc, char **argv, char **env) {
	(void)argc;
	(void)argv;
	//char command[] = "echo a && echo b | echo c";
	//char command[] = "echo a | echo c >> f | echo m > f2 | cat ll";
	//char command[] = "cat << EOF << hello << bye > f << EOF > g";
	//char command[] = "(echo a && echo b) && (echo d && ((echo bruh  > 3 && echo rawr) && echo r)) >> f > c | cat";
	//char command[] = "(echo a v && echo \"hello world\") && (echo bruh && echo rawr) | cat";
	//char command[] = "cat << EOF << hello << bye > f > g";
	//char command[] = "cat << EOF << hello << bye > f << EOF";
	char command[] = "(echo $USER'$user' && echo b)";
	int count;
	t_env *lst_env = spy_env(env);
	t_token *t = tokenize(ft_strdup(command), lst_env);
	print_list_tok(t);
	t_cmd *tokens = transform_into_tab(t, &count);
	int i = 0;
	printf("----------------------------------------\n");
	while(tokens && i < count)
	{
		printf("%s\t", tokens[i].content);
		i++;
	}
	printf("\n-----------count : %d--------------------\n", count);
	t_border *b = malloc(sizeof(t_border));
	int n = 0;
	b->start = &n;
	b->end = count - 1;
	t_ast* root = build_ast(tokens, b);
	printAST(root);
	printf("\n");
	exec_ast(root, STDIN_FILENO, STDOUT_FILENO, lst_env);
	free_ast(root);
	i = 0;
	while (i < count)
		free(tokens[i++].content);
	free(tokens);
	return 0;
}
