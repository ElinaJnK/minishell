#include "minishell.h"

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

t_token	*lst_cut(t_token *lst_tok, int s, int e)
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
	printf("WTF HAPPENING HERE in cut :\n");
	print_list_tok(cut);
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

	while (d && i < e)
	{
		printf("here f\n");
		if (d->type == 8)
		{
			if (p == 0)
				s = i + 1;
			p++;
		}
		else if (d->type == 9)
		{
			p--;
			if (p == 0)
			{
				printf("fff\n");
				r = new_node(lst_cut(d, s, s + 1));
				printf("i : %d s : % d e : %d \n", i, s, e);
				r->left = create_ast(lst_cut(lst_tok, s, i));
				printf("i : %d e : %d \n", i, e);
				r->right = create_ast(lst_cut(lst_tok, i + 2, e));
				break;
			}
		}
		else if (p == 0 && (d->type == 1 || d->type == 2)) // Operator
		{
			r = new_node(lst_cut(d, s, s + 1));
			r->left = create_ast(lst_cut(lst_tok, s, i));
			r->right = create_ast(lst_cut(lst_tok, i + 1, e));
			break;
		}

		i++;
		d = d->next;
	}

	if (r == NULL)
	{
		printf("here\n");
		r = new_node(lst_tok);
		r->left = NULL;
		r->right = NULL;
	}

	return (r);
}

void	print_ast(t_ast *root, char c)
{
	if (root == NULL)
		return ;

	print_ast(root->left, c + 1);
	printf("%c", c);
	print_list_tok(root->tok);
	print_ast(root->right, c + 1);
}

void	print_ast_tree(t_ast* root, int level)
{
	if (root == NULL)
	{
		return ;
	}

	// Print right subtree
	print_ast_tree(root->right, level + 1);

	// Print indentation based on the level
	for (int i = 0; i < level; i++) {
		printf("   ");
	}

	// Print node value
	printf("|__");
	print_list_tok(root->tok);
	printf("\n");

	// Print left subtree
	print_ast_tree(root->left, level + 1);
}



// int	main(void)
// {
// 	t_ast	*a;
// 	t_token	*lst_tok;

// 	// lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
// 	//lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
// 	//lst_tok = tokenize("((echo bye) && cd dossier||echo he'l'lo )| cat -e");
// 	//lst_tok = tokenize("echo hello && echo world || echo hello 2world");
// 	lst_tok = tokenize("a&&b||c");
// 	printf("lst_tok type : %d\n", lst_tok->type);
// 	print_list_tok(lst_tok);
// 	a = create_ast(lst_tok);
// 	print_ast_tree(a, 5);
// 	print_ast(a, 'A');
// 	free_lst_tok(&lst_tok);
// 	free_ast(a);
// 	return (0);
// }
