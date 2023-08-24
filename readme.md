# README - minishell
## _Avec Karima Sadykova (aka Karimo)_
_Merci à Samy Naji pour les nombreux tests (dont test_commands.py) et à Akadil Kalimoldayev pour le pipe_

Résumé de tous les steps à prendre pour mini shell. Ce readme n'est pas final et il contient parfois des idées que nous avons changé par la suite. Par exemple la partie tokenize a été beaucoup plus longue que prévu initiallement. 
Allez verifier le code et contactez nous si vous trouvez un leak :)

Tout d’abord voici les indications que donne le bash:

The following is a brief description of the shell’s operation when it reads and executes a command. Basically, the shell does the following:
- Reads its input from a file (see Shell Scripts), from a string supplied as an argument to the -c invocation option (see Invoking Bash), or from the user’s terminal.
- Breaks the input into words and operators, obeying the quoting rules described in Quoting. These tokens are separated by metacharacters. Alias expansion is performed by this step (see Aliases).
- Parses the tokens into simple and compound commands (see Shell Commands).
- Performs the various shell expansions (see Shell Expansions), breaking the expanded tokens into lists of filenames (see Filename Expansion) and commands and arguments.
- Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list.
- Executes the command (see Executing Commands).
Optionally waits for the command to complete and collects its exit status (see Exit Status).

Commençons avec ceci:

- Reads its input from a file (see Shell Scripts), from a string supplied as an argument to the -c invocation option (see Invoking Bash), or from the user’s terminal.

Une vérification d'anciens projets nous indique que ./script.sh ne devra pas executer ce qu'il y a dans le script.
On peut donc prévoir une fonction:
```c
read_stdin()
```
qui lira simplement l'ensemble de la ligne.

Allez, maintenant on s'amuse:
- Breaks the input into words and operators, obeying the quoting rules described in Quoting. These tokens are separated by metacharacters. Alias expansion is performed by this step (see Aliases).

1. Initialize an empty list of tokens.
2. Initialize an empty string to store the current token.
3. Initialize a flag to indicate if the current token is within quotes (0 if outside quotes, 1 if within double quotes, 2 if within single quotes).
4. Iterate through each character in the command line:
	a. If the character is a space or tab:
		- If the flag is 0 (outside quotes) and the current token is not empty:
			- Add the current token to the list of tokens.
			- Reset the current token to an empty string.
		- If the flag is 1 or 2 (within quotes):
			- Append the space or tab character to the current token.
	b. If the character is a double quote (") and the flag is 0:
		- Set the flag to 1 (within double quotes).
	c. If the character is a single quote (') and the flag is 0:
		- Set the flag to 2 (within single quotes).
	d. If the character is a backslash (\) and the flag is not 2 (not within single quotes):
		- If the next character exists, append it to the current token and skip the next iteration.
	e. If none of the above conditions are met:
		- Append the character to the current token.
5. If the current token is not empty:
	- Add the current token to the list of tokens.
6. Return the list of tokens.

Ici on a donc décidé de partir sur une liste chaînée:
```c
typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}		t_token;
```
pour avoir qqch qui ressemblera à ça:
```sh
> ec'ho' nana && (cd dossier || echo yum)
# gives
> [echo, nana, &&, (, cd, dossier, ||, echo, yum, )]
```
en gros chaque chose séparée par un espace est un élement de la liste.
----------------------------------------------------------------------------
```sh
Peut-etre prendre toute la chaine et appeler une fonction qui va renvoyer string analyse sans guillements ni apostrophes.

Apres cela tokenizer et creer une liste chainee du type t_token

Creer fonction check_com qui regarde si tt les commandes obtenues sont valides et leur arguments (?)
```
----------------------------------------------------------------------------
Dans le cas spécifique où l'on a:
```
ech"o y"n
bash: echo yn: command not found
```
l'espace n'est ici pas le séparateur !
En effet entre quotes, il faut juste tout prendre et une fois que l'on sort des quotes on reprend un comportement normal.
Attention aussi à ça:
```
ech"o " yn
```
en effet ça ne marche pas car on envoie echo[espace] à bash ce qui n'est pas une commande qui existe.

On a donc maintenant chaque élement dans notre liste avec leur type, on peut par exemple dire que:
```c

int	is_op(char *line, int i)
{
	if (ft_strncmp(line + i, "&&", 2) == 0)
		return (1);
	else if (ft_strncmp(line + i, "||", 2) == 0)
		return (2);
	return (0);
}

int	is_fb(char *line, int i)
{
	else if (ft_strncmp(line + i, "|", 1) == 0)
		return (3);
  	else if (ft_strncmp(line + i, ">>", 2) == 0)
		return (6);
	else if (ft_strncmp(line + i, "<<", 2) == 0)
		return (7);
	else if (ft_strncmp(line + i, ">", 1) == 0)
		return (4);
	else if (ft_strncmp(line + i, "<", 1) == 0)
		return (5);
	else if (ft_strncmp(line + i, "(", 1) == 0)
		return (8);
	else if (ft_strncmp(line + i, ")", 1) == 0)
		return (9);
	else if (ft_strncmp(line + i, " ", 1) == 0)
		return (10);
	return (0);
}
```

P. S. : avant de passer à la partie suivante, pensez à gérer les here_doc ! En effet, c'est plus facile de le faire dans le parsing que dans l'execution. 

Eh mais alors ensuite ?

- Parses the tokens into simple and compound commands (see Shell Commands).
Ici on guette. Ici on analyse, ici on cherche: les parenthesès !
Tout d'abord, on pensait simplement refaire une nouvelle liste qui séparerait les compound commands (entre parenthèses) et les simple commands (comme echo a par exemple). Cependant, on c'est vite rendu compte que c'était assez difficilement gérable.
Alors, on a créé des arbres binaires :D.
Le fonctionnement de l'algo est assez simple:
- on regarde quel type de métacharactère on a : <, <<, >, >>, &&, || ou (, )
- si on a une parenthèse, on va chercher la dernière parenthèse puis lancer un appel récursif sur ce nouveau bout de commande
- si on a des parenthèses dans des parenthèses on va relancer un appel récursif
- ces appels récursifs vont créer des "subtree" qui vont ensuite être ajoutés à l'arbre principal.

Ce bout de code peut être trouvé dans /parsing/build_ast.c et /utils/ast_utils.c
Finalement pour cette commande :
```sh
> (1 && 2) || ((3 || 4 || 5) && (6 && 7)) && 8
> [1, &&, 2] -> [||] -> [ [3, ||, 4, ||, 5] -> [&&] -> [6, &&, 7] ] -> [&&] -> [8]
```
On aura un arbre qui ressemble à ça :
```
	||
        /  \
      &&    &&
     /  \   / \
    1    2 ||   &&
         / | \ / \
        3  4  5 6  7
```

- Performs the various shell expansions (see Shell Expansions), breaking the expanded tokens into lists of filenames (see Filename Expansion) and commands and arguments.
```
#chatjpp
Perform variable expansion:
The shell performs variable expansion for the environment variables used in the command ($INPUT_FILE, $OUTPUT_FILE, $LOG_FILE), replacing them with their respective values.
```

On a ensuite décidé de faire les shell expansions biennnnnn avant au début de la tokenization. Ça allait beaucoup mieux avec notre code jusque là (et puis ce n'est pas très pratique de le faire dans un arbre).

Redirection time, comme ce minishell qui me donne envie de me rediriger en cap patisserie+plomberie:
- Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list.

Ici on fait nos petits dup2 j'imagine, on prend les piti fichier et on remplace les piti files descriptor par les piti fichier <3
- Executes the command (see Executing Commands).
Optionally waits for the command to complete and collects its exit status (see Exit Status).

Ça, c'est dans /exec/exec_ast.c comme quand on a construit l'abre, on va maintenant le parcourir et appliquer différentes fonctions selon les noeuds differents. Puis finalement executer les feuilles.

On gère aussi les builtins au niveau des feuilles, avec quelques vérifications :
- Dans export, verifier que chaque variable qu'on rajoute respecte la specification posix:
environment variable names must consist solely of uppercase letters, digits, and underscores (_), and they must not start with a digit.

Finalement on a géré les signaux, le fichier est dans /shell/signal.c

A la fin du projet, nous allons aussi inclure certains tests qui semblent un peu contre intuitifs (update : ils sont maintenant dans tests.c :)). A bientôt :D
	
