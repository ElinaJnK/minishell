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

t_all	*build_all(t_env *lst_env)
{
	t_all		*all;
	t_border	*b;

	b = malloc(sizeof(t_border));
	if (!b)
		return (NULL);
	all = malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	all->ast = NULL;
	all->b = b;
	all->cmd = NULL;
	all->env = lst_env;
	all->count = 0;
	all->n_pipes = 0;
	all->prompt_good = ft_strdup("\001\033[35m\002(▼・ᴥ・▼) $ \001\033[0m\002");
	all->prompt_bad = ft_strdup("\001\033[31m\002ლ(́◉◞౪◟◉‵ლ) $ \001\033[0m\002");
	return (all);
}

void	init_border(t_all **all, int *count)
{
	(*all)->count = *count;
	(*all)->b->end = *count - 1;
	*count = 0;
	(*all)->b->start = count;
}

void	just_do_it(t_all **all, char *line, int count)
{
	t_cmd		*cmds;
	t_ast		*root;
	t_token		*t;

	cmds = NULL;
	t = NULL;
	(*all)->n_pipes = 0;
	t = tokenize(line, (*all)->env);
	t = tokenize_bise(t);
	t = tokenize_crise(t);
	if (t && (*all)->env)
		cmds = transform_into_tab(t, &count, (*all)->env, all);
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
		if (cmds)
			free_cmds(cmds, (*all)->count);
		if (root)
			free_ast(root);
	}
}

void	tchitat_stdin(t_all **all)
{
	char		*line;
	int			count;

	while (1)
	{
		signal_prompt();
		if (*(exit_status()) == 0)
			line = readline((*all)->prompt_good);
		else
			line = readline((*all)->prompt_bad);
		if (!line)
			return ;
		add_history(line);
		count = 0;
		if (*line == '\n' || *line == '\0')
		{
			free(line);
			line = NULL;
		}
		signal_exec();
		if (line)
			just_do_it(all, line, count);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env		*lst_env;
	t_all		*all;

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
	if (!lst_env)
		return (failure("Error in malloc allocation"), 1);
	all = build_all(lst_env);
	if (!all)
		return (failure("Error in malloc allocation"), free_lst_env(&lst_env),
			*exit_status());
	tchitat_stdin(&all);
	free_all(all);
	//rl_clear_history();
	return (*exit_status());
}
