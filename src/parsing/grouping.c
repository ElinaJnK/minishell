#include "minishell.h"

t_token	*cut(t_token *lst_tok, int s, int e)
{
	t_token	*cut;
	t_token	*tmp;
	int		i;

	tmp = lst_tok;
	cut = NULL;
	i = e - s;
	while (tmp && s-- > 0)
		tmp = tmp->next;
	while (tmp && i-- > 0)
	{
		add_back_tok(&cut, new_token(tmp->content, tmp->type));
		tmp = tmp->next;
	}
	return (cut);
}

int	lstsize(t_token *lst)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

t_ast	*create_ast(t_token *lst_tok)
{
	if (lst_tok == NULL)
	{
		failure("lst_tok is NULL");
		return NULL;
	}

	t_ast *r = NULL;
	t_token *d = lst_tok;
	int p = 0;
	int s = 0;
	int e = lstsize(lst_tok);
	int i = s;

	while (d && i <= e)
	{
		if (d->type == 8) // Opening parenthesis
		{
			if (p == 0)
				s = i + 1; // Start of the subexpression
			p++;
		}
		else if (d->type == 9) // Closing parenthesis
		{
			p--;
			if (p == 0)
			{
				// Create a new AST node for the subexpression
				r = new_node(d);
				
				r->left = create_ast(cut(lst_tok, s, i - 1));
				r->right = create_ast(cut(lst_tok, i + 1, e));
				break;
			}
		}
		else if (p == 0 && (d->type == 1 || d->type == 2)) // Operator
		{
			r = new_node(d);
			r->left = create_ast(cut(lst_tok, s, i - 1));
			r->right = create_ast(cut(lst_tok, i + 1, e));
			break;
		}

		i++;
		d = d->next;
	}

	if (r == NULL)
	{
		r = new_node(lst_tok);
		r->left = NULL;
		r->right = NULL;
	}

	return r;
}

void	print_list_tok(t_token *lst_tok)
{
	t_token	*temp;

	temp = lst_tok;
	while (temp)
	{
		//printf("[%s, %d]->", temp->content, temp->type);
		printf("[%s]->", temp->content);
		temp = temp->next;
	}
	printf("\n");
}

void	print_ast(t_ast *root)
{
	if (root == NULL)
		return ;

	print_ast(root->left);
	print_list_tok(root->tok);
	print_ast(root->right);
}

int	main(void)
{
	t_ast	*a;
	t_token	*lst_tok;

	// lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
	//lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
	lst_tok = tokenize("((echo bye) && cd dossier||echo he'l'lo ) || cat -e");
	print_list_tok(lst_tok);
	a = create_ast(lst_tok);
	print_ast(a);
	free_lst_tok(&lst_tok);
	free_ast(a);
	return (0);
}
