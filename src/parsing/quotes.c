#include "../../minishell.h"

void	dquotes(char *line, int *i)
{
	int	end;

	end = *i;
	while (line[end] != '"')
	{
		if (line[end] == '$' && expand)
			line = expansion(line, &end);
		else
			end++;
	}
	if (line[end - 1] != '"')
		failure("double quotes not complete motherfucker");
	*i = end;
}

void	squotes(char *line, int *i)
{
	int	end;

	end = *i;
	while (line[end] != '\'')
		end++;
	if (line[end - 1] != '\'')
		failure("single quotes not complete motherfucker");
	*i = end;
}
