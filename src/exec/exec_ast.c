#include <minishell.h>

void	exec_and(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	exec_ast(root->left, input_fd, output_fd, all);
	if (*exit_status() == 0)
		exec_ast(root->right, input_fd, output_fd, all);
}

void	exec_or(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	exec_ast(root->left, input_fd, output_fd, all);
	if (*exit_status() != 0)
		exec_ast(root->right, input_fd, output_fd, all);
}

void	exec_pipe(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	int	pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		failure_exec("fork error");
	exec_ast(root->left, input_fd, pipe_fds[1], all);
	close(pipe_fds[1]);
	exec_ast(root->right, pipe_fds[0], output_fd, all);
}

void	exec_redir(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	if (root->left)
	{
		if (root->cmd->input != STDIN_FILENO && root->cmd->output
			!= STDOUT_FILENO)
			exec_ast(root->left, root->cmd->input, root->cmd->output, all);
		else if (root->cmd->input != STDIN_FILENO)
			exec_ast(root->left, root->cmd->input, output_fd, all);
		else if (root->cmd->output != STDOUT_FILENO)
			exec_ast(root->left, input_fd, root->cmd->output, all);
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
