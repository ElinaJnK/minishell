#include "minishell.h"

while (parcourir liste chainee)
{
	pour chaque element de la liste chainee:
		fork();
		if (pid == 0)
		{
			//execve
		}
		else
		{
			waitpid(pid, &status, 0);
			if (status == 0)
				//execve
		}
}