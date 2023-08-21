#include "minishell.h"

/*-------------------------------DELETE------------------------------*/
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

void print_list_tok(t_token *lst_tok)
{
	t_token *temp;
	int		size = 0;

	printf("-------------------------------\n");
	temp = lst_tok;
	while (temp)
	{
		printf("[%s, %d]->", temp->content, temp->type);
		//printf("[%s]->", temp->content);
		temp = temp->next;
		size++;
		//printf("\n");
	}
	printf("\n-----size of lst_tok : %d------\n", size);
}

void	print_cmds(t_cmd *cmds, int count)
{
	int	j = 0;
	printf("\n------------------------------\n");
	while (j < count)
	{
		printf("cmds[%d] : %s\n", j, cmds[j].content);
		int a = 0;
		while (cmds[j].args && a < cmds[j].nb_args + 1)
		{
			printf("cmds[%d].args[%d] : %s\n", j, a, cmds[j].args[a]);
			a++;
		}
		j++;
	}
	printf("------------------------------\n");
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
/*-------------------------------DELETE------------------------------*/