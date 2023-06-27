#include "../../minishell.h"

//eventually do this code (if we follow from the guys):
/*
operator :
- 1 : && is a token but &
- 2 : ||
- 3 : |
redirection symbol :
- 4 : >
- 5 : <
- 6 : >>
- 7 : << 
- 8 : cmd
- 9 : args
*/

t_token	*get_args_tok(char *line, int *start)
{
	t_token	*tok;
	int		end;

	end = *start;
	while (line && line[end] != '\0' && !is_op(line, end))
	{
		if (line[i] == '"')
			dquotes(line, &end);
		if (line[i] == '\'')
			squotes(line, &end);
		else
			end++;
	}
	tok = new_token(ft_substr(line, start, end), 9);
	return (tok);
}

t_token	*get_command_tok(char *line, int *start)
{
	t_token	*tok;
	int		end;

	end = *start;
	while (line && line[end] != '\0'
		&& line[end] != ' ' && !is_op(line, end))
	{
		if (line[i] == '"')
			dquotes(line, &end);
		if (line[i] == '\'')
			squotes(line, &end);
		else
			end++;
	}
	tok = new_token(ft_substr(line, start, end), 8);
	if (line && line[end] == ' ')
		add_back_tok(tok, get_args_tok(line, &end));
	return (tok);
}

t_token	*get_op_tok(char *line, int *start)
{
	t_token	*tok;
	int		type;

	type = is_op(line, end);
	if (type == 1 || type == 2 || type == 6 || type == 7)
		len_op += 2;
	else
		len_op += 1;
	tok = new_token(ft_substr(line, start, start + len_op), type);
	*start += len_op;
	return (tok);
}

t_token	*tokenize(char *line)
{
	t_token	*lst_tok;
	t_token	*tok;
	int		i;

	tok = NULL;
	lst_tok = NULL;
	i = 0;
	while (i < ft_strlen(line))
	{
		if (is_op(line, i))
			tok = get_op_tok(line, &i);
		else
			tok = get_command_tok(line, &i);
		add_back_tok(lst_tok, tok);
	}
	return (lst_tok);
}

t_abr	*parsing(char *line)
{
	t_token	*lst_tok;
	t_abr	*a;

	lst_tok = tokenize(line);
	//lst_tok = check_syntax(lst_tok);
	//a = create_abr(lst_tok);
	return (a);
}
