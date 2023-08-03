#include "minishell.h"

int	get_type(char *tok)
{
	int	op;
	int	meta;

	if (!tok)
		return (-1);
	op = is_op(tok);
	meta = is_fb(tok);
	if (op)
		return (op);
	if (meta)
		return (meta);
	return (CMD);
}

int	is_op(char *line)
{
	if (ft_strncmp(line, "&&", 2) == 0)
		return (AND);
	else if (ft_strncmp(line, "||", 2) == 0)
		return (OR);
	return (0);
}

int	is_fb(char *line)
{
	if (ft_strncmp(line, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(line, ">>", 2) == 0)
		return (DREDIR);
	else if (ft_strncmp(line, "<<", 2) == 0)
		return (DREDIR2);
	else if (ft_strncmp(line, ">", 1) == 0)
		return (REDIR);
	else if (ft_strncmp(line, "<", 1) == 0)
		return (REDIR2);
	else if (ft_strncmp(line, "(", 1) == 0)
		return (OPEN_PAR);
	else if (ft_strncmp(line, ")", 1) == 0)
		return (CLOSE_PAR);
	return (CMD);
}
