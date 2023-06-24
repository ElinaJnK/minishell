// #include "../../minishell.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

typedef struct s_abr
{
	char			data;
	struct s_abr	*g;
	struct s_abr	*d;
}		t_abr;

int	is_op(char op)
{
	return (op == '&' || op == '|');
}

t_abr	*create_cell(char data)
{
	t_abr	*n;

	n = (t_abr *)malloc(sizeof(t_abr));
	n->data = data;
	n->g = NULL;
	n->d = NULL;
	return (n);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

t_abr	*create_abr(char *expr, int start, int end)
{
	int		bc;
	int		i;
	t_abr	*root;

	if (start > end)
		return (NULL);
	if (end == start)
	{
		return (create_cell(expr[start]));
	}
	bc = 0;
	root = NULL;
	i = end;
	int s, e;
	s = start;
	e = end;
	while (i >= start)
	{
		printf("expr[%d] : %c\n", i, expr[i]);
		if (expr[i] == ')')
		{
			if (bc == 0)
			{
				e = i - 1;
				root->g = create_abr(expr, s, i - 1);
			}
			bc++;
		}
		else if (expr[i] == '(')
		{
			bc--;
			if (bc == 0)
			{
				s = i + 1;
				root->d = create_abr(expr, i + 1, e);
			}
		}
		else if (bc == 0 && is_op(expr[i]))
		{
			root = create_cell(expr[i]);
			if (root == NULL)
			{
				printf("Memory allocation failed.\n");
				exit(1);
			}
			printf("data : %c\n", root->data);
			root->d = create_abr(expr, i + 1, end);
			root->g = create_abr(expr, start, i - 1);
			break ;
		}
		i--;
	}

	return (root);
}

void	free_abr(t_abr *a)
{
	if (!a)
		return ;
	free_abr(a->g);
	free_abr(a->d);
	free(a);
}

void	print_abr(t_abr *root)
{
	if (root == NULL)
		return ;

	if (is_op(root->data))
		printf("(");

	print_abr(root->g);
	printf("%c", root->data);
	print_abr(root->d);

	if (is_op(root->data))
		printf(")");
}

int	main(int ac, char **av)
{
	int		i;
	t_abr	*a;

	i = 0;
	a = create_abr(av[1], 0, ft_strlen(av[1]) - 1);
	print_abr(a);
	free_abr(a);
	return (0);
}
