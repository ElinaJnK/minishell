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

void	failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

t_all	*build_all(t_cmd *tokens, t_ast *root, t_env *lst_env, int count)
{
	t_all	*all;

	all = malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	all->ast = root;
	all->cmd = tokens;
	all->env = lst_env;
	all->count = count;
	return (all);
}

t_cmd	*return_command(char *line, t_all **all, int *count)
{
	t_cmd		*cmds;
	t_token		*t;

	cmds = NULL;
	t = NULL;
	t = tokenize(line, (*all)->env);
	t = tokenize_bise(t);
	t = tokenize_crise(t);
	if (t && (*all)->env)
		cmds = transform_into_tab(t, count, (*all)->env);
	return (cmds);
}

void	init_border(t_all **all, int *count)
{
	(*all)->count = *count;
	(*all)->b->end = *count - 1;
	*count = 0;
	(*all)->b->start = count;
}

void	tchitat_stdin(t_all **all)
{
	char		*line;
	t_cmd		*cmds;
	int			count;
	t_ast		*root;

	//catch_the_signal();
	while (1)
	{
		//add_history(line);
		root = NULL;
		count = 0;
		line = readline("(▼・ᴥ・▼)$ ");
		cmds = return_command(line, all, &count);
		if (cmds)
		{
			init_border(all, &count);
			root = build_ast(cmds, (*all)->b);
			if (root && (*all)->env)
			{
				(*all)->cmd = cmds;
				(*all)->ast = root;
				exec_ast((*all)->ast, STDIN_FILENO, STDOUT_FILENO, *all);
			}
			free_cmds(cmds, count);
			if (root)
				free_ast(root);
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_env		*lst_env;
	t_all		*all;
	t_border	*b;

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
	b = malloc(sizeof(t_border));
	if (!b)
		return (failure("Error in malloc allocation"), free_lst_env(&lst_env),
			*exit_status());
	all = build_all(NULL, NULL, lst_env, 0);
	all->b = b;
	tchitat_stdin(&all);
	free_all(all);
	//rl_clear_history();
	return (*exit_status());
}
