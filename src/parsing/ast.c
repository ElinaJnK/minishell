#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

t_ast *create_node(t_cmd *cmd)
{
	t_ast*	node;

	node = (t_ast *) malloc(sizeof(t_ast));
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast* build_ast(t_cmd *tokens, int start, int end) {
	t_ast	*root;
	t_ast	*current;
	int		p_open;
	int		p_close;
	int		i;
	int		j;
	t_ast* subtree;
	t_ast* node;
	t_ast* tmp;

	if (start > end)
		return (NULL);
	root = NULL;
	current = NULL;
	i = start;
	while (i <= end)
	{
		if (!root)
			root = current;
		if (tokens[i].type == OPEN_PAR)
		{
			p_open = 1;
			p_close = 0;
			j = i + 1;
			while (j <= end)
			{
				if (tokens[j].type == OPEN_PAR)
					p_open++;
				else if (tokens[j].type == CLOSE_PAR)
					p_close++;
				if (p_open == p_close)
					break ;
				j++;
			}
			subtree = build_ast(tokens, i + 1, j - 1);
			if (current == NULL)
				root = subtree;
			else
				current->right = subtree;
			i = j;
		}
		else if (tokens[i].type == OR || tokens[i].type == AND
			|| tokens[i].type == PIPE)
		{
			node = create_node(&tokens[i]);
			node->left = root;
			current = node;
			root = NULL;
		}
		else if (tokens[i].type == REDIR || tokens[i].type == REDIR2
			|| tokens[i].type == DREDIR || tokens[i].type == DREDIR2)
		{
			node = create_node(&tokens[i]);
			if (current == NULL)
			{
				node->left = root;
				current = node;
				root = node;
			}
			else
			{
				tmp = current;
				while (tmp && tmp->right)
					tmp = tmp->right;
				current->right = node;
				node->left = tmp;
				current = node;
			}
		}
		else
		{
			node = create_node(&tokens[i]);
			if (current == NULL)
				root = node;
			else
				current->right = node;
		}
		i++;
	}
	if (root == NULL)
		root = current;
	return (root);
}

// Function to tokenize the command string
char** tokenizeCommand(char* command, int* count) {
	char* token;
	char** tokens = NULL;
	int i = 0;

	token = strtok(command, " ");
	while (token != NULL) {
		tokens = realloc(tokens, sizeof(char*) * (i + 1));
		tokens[i] = token;
		i++;

		token = strtok(NULL, " ");
	}

	*count = i;
	return tokens;
}

// Function to recursively free the memory of the AST
void free_ast(t_ast* node) {
	if (node == NULL)
		return;
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

// Function to print the AST in postfix order (for demonstration purposes)
void printASTb(t_ast* node) {
	if (node == NULL)
		return;
	printASTb(node->left);
	printf("%s ", node->cmd->content);
	int i = 0;
	while (node->cmd->args && i < node->cmd->nb_args)
	{
		printf("%s", node->cmd->args[i]);
		i++;
	}
	printASTb(node->right);
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
	while (node->cmd->args && i < node->cmd->nb_args)
	{
		printf("%s", node->cmd->args[i]);
		i++;
	}
	printf("\n");
	printASTHelper(node->left, depth, 0);
}

void printAST(t_ast* root) {
	printf("root: %s\n", root->cmd->content);
    printASTHelper(root, 0, 0);
}

// int main(int argc, char **argv, char **env) {
// 	//char command[] = "cmd1 || ( cmd2 && ( cmd3 || cmd4 ) )";
// 	// char command[] = "( 1 && 2 ) || ( ( 3 || 4 || 5 ) && ( 6 || 7 ) ) && 8";
// 	//char command[] = "( cmd3 || cmd4 ) && cmd5";
// 	//char command[] = "cmd1 || ( cmd3 || cmd4 ) && cmd5";
// 	//char command[] = "( cmd3 || cmd4 ) | cmd5";
// 	//char command[] = "cmd1 > e && cmd2 && cmd3 > d > f3";
// 	(void)argc;
// 	(void)argv;
// 	printf("%s\n", env[0]);
// 	char command[] = "echo a > fichier1 && echo c && echo rawr > fichier2 > fichier3";
// 	int count;
// 	t_token *t = tokenize(command);
// 	t_cmd *tokens = transform_into_tab(t, &count);
// 	int i = 0;
// 	while(tokens && i < count)
// 	{
// 		printf("%s\n", tokens[i].content);
// 		i++;
// 	}
// 	printf("count: %d\n", count);
// 	t_ast* root = build_ast(tokens, 0, count - 1);
// 	printAST(root);
// 	printf("\n");

// 	// Cleanup: Free the memory of the AST and tokens
// 	free_ast(root);
// 	i = 0;
// 	while (i < count)
// 		free(tokens[i++].content);
// 	free(tokens);
// 	return 0;
// }
