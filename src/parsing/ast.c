#include "minishell.h"

void	ast_in_par(t_cmd *tokens, t_ast **root, t_ast **cur, t_border *b)
{
	int		j;
	int		n;
	int		p_count;
	t_ast	*subtree;

	if (*root || *cur)
		;
	p_count = 1;
	j = *b->start;
	while (j++ <= b->end)
	{
		if (tokens[j].type == OPEN_PAR)
			p_count++;
		else if (tokens[j].type == CLOSE_PAR)
			p_count--;
		if (p_count == 0)
			break ;
	}
	n = *b->start + 1;
	subtree = build_ast(tokens, &(t_border){&n, j - 1});
	if (*cur == NULL)
		*root = subtree;
	else
		(*cur)->right = subtree;
	*b->start = j;
}

void	ast_redir(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*node;
	t_ast	*tmp;

	if (*root || *current)
		;
	tmp = NULL;
	node = create_node(&tokens[start]);
	if (*current == NULL)
	{
		node->left = *root;
		*current = node;
		*root = node;
	}
	else
	{
		tmp = (*current)->right;
		while (tmp && tmp->right
			&& !(tmp->cmd->type >= 1 && tmp->cmd->type <= 3))
			tmp = tmp->right;
		(*current)->right = node;
		node->left = tmp;
		*current = node;
	}
}

void	ast_cmd(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*node;

	if (*root || *current)
		;
	node = create_node(&tokens[start]);
	if (*current == NULL)
		*root = node;
	else
		(*current)->right = node;
}

void	ast_op(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*node;

	node = create_node(&tokens[start]);
	node->left = *root;
	*current = node;
	*root = NULL;
}

void	ast_pipe(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*tmp;
	t_ast	*node;

	tmp = NULL;
	node = NULL;
	if ((*root)->cmd->type == AND || (*root)->cmd->type == OR)
	{
		tmp = (*root)->right;
		node = create_node(&tokens[start]);
		(*root)->right = node;
		node->left = tmp;
		*current = node;
	}
	else
		ast_op(tokens, root, current, start);
}

t_ast	*build_ast(t_cmd *tokens, t_border *b)
{
	t_ast	*root;
	t_ast	*current;
	int		i;

	if (*b->start > b->end)
		return (NULL);
	root = NULL;
	current = NULL;
	i = *b->start;
	while (i <= b->end)
	{
		if (!root)
			root = current;
		if (tokens[i].type == OPEN_PAR)
			ast_in_par(tokens, &root, &current, &(t_border){&i, b->end});
		else if (tokens[i].type == AND || tokens[i].type == OR)
			ast_op(tokens, &root, &current, i);
		else if ((tokens[i].type >= REDIR && tokens[i].type <= DREDIR2_E))
			ast_redir(tokens, &root, &current, i);
		else if (tokens[i].type == PIPE)
			ast_pipe(tokens, &root, &current, i);
		else
			ast_cmd(tokens, &root, &current, i);
		i++;
	}
	return (root);
}

/*------------------------------------FUNCTIONS TO DELETE-----------------------------------------------------*/

//Function to print the AST in postfix order (for demonstration purposes)
// void printASTb(t_ast* node) {
// 	if (node == NULL)
// 		return;
// 	printASTb(node->left);
// 	printf("%s ", node->cmd->content);
// 	int i = 0;
// 	while (node->cmd->args && i < node->cmd->nb_args)
// 	{
// 		printf("%s", node->cmd->args[i]);
// 		i++;
// 	}
// 	printASTb(node->right);
// }

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
// 	while (node->cmd->args && i < node->cmd->nb_args + 1)
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
// 	//char command[] = "cmd arg << LIMITER | cmd1 >> file | echo pipe";
// 	//char command[] = "ls && cat << EOF1 -e << EOF2";
// 	//char command[] = "((ls > f -la) > f && echo a << f) << EOF | cat";
// 	char command[] = "ls << EOF -la";
// 	//char command[] = "> f ls > g -la < input";
// 	int count;
// 	t_env *lst_env = spy_env(env);
// 	t_token *t = tokenize(command, lst_env);
// 	print_list_tok(t);

// 	t_token	*lst_bise = tokenize_bise(t);
// 	print_list_tok(lst_bise);
	
// 	t_cmd *tokens = transform_into_tab(lst_bise, &count, lst_env);
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
// 	// printf("\n");

// 	// free_ast(root);
// 	i = 0;
// 	while (i < count)
// 		free(tokens[i++].content);
// 	free(tokens);
// 	return 0;
// }

/*------------------------------------FUNCTIONS TO DELETE-----------------------------------------------------*/