#include "../../minishell.h"

//eventually do this code (if we follow from the guys):
/*
- 0 : &&
- 1 : ||
- 2 : >
- 3 : */
char	*divide(char *line)
{

}

t_token	*parsing(char *line)
{
	t_token	*tok;
	int		i;
	int		end;
	int		start;

	i = 0;
	start = 0;
	end = 0;
	while (i < ft_strlen(line))
	{
		end = start;
		while (!is_token(line[i++]))
			end++;
		tok = add_back_tok(line, start, end);
		i++;
	}
}
// on peut reprendre cette fonction mais on doit faire le parsing sur la ligne
t_token	*read_stdin(char *limiter)
{
	char	*line;
	t_token	*tok;

	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line && ft_strncmp(line, limiter, ft_max(ft_strlen(line) - 1,
				ft_strlen(limiter))) != 0)
	{
		tok = parsing(line);
		free(line);
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	return (tok);
}
