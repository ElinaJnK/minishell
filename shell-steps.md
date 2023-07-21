# Readme for minishell
Résumé de tous les steps à prendre pour mini shell (parce que je n’ai pas envie de le commencer seule):

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
	else if (ft_strncmp(line + i, ">", 1) == 0)
		return (4);
	else if (ft_strncmp(line + i, "<", 1) == 0)
		return (5);
	else if (ft_strncmp(line + i, ">>", 2) == 0)
		return (6);
	else if (ft_strncmp(line + i, "<<", 2) == 0)
		return (7);
	else if (ft_strncmp(line + i, "(", 1) == 0)
		return (8);
	else if (ft_strncmp(line + i, ")", 1) == 0)
		return (9);
	else if (ft_strncmp(line + i, " ", 1) == 0)
		return (10);
	return (0);
}
éventuellement on peut changer cette analyse.

Eh mais alors ensuite ?

- Parses the tokens into simple and compound commands (see Shell Commands).
Ici on guette. Ici on analyse, ici on cherche: les parenthesès !
En effet on va ici regarder quel sont les compound commands (entre parenthese), on peut aussi s'occuper de bien verifier le nombre de parentheses (on peut le faire avant aussi).
On regroupe dans une nouvelle liste, les simple commands d'un côté et les compound commands de l'autre.

Ensuite, dans une boucle, quand on entrera dans la partie compound command, BAM on fait un fork.
On a donc:
```sh
> [echo, nana, &&, (, cd, dossier, ||, echo, yum, )]
# and we do
> [echo nana, &&, (cd dossier || echo yum)]
# and then recursively in the fork (later possibly)
> [cd dossier, ||, echo yum]
# in the end, this step gives us
# if we take this structure:
typedef struct s_cmd
{
	t_token			*lst_tok;
	t_cmd			*lst_com;
	int				type;
	struct s_cmd	*next;
}		t_cmd;
typedef struct s_cmd
{
	t_token			*content;
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}		t_cmd;
> [echo nana, cmd] -> [&&, meta] -> [(cd dossier || echo yum), cmpound_cmd]
```

```sh
> (1 && 2) || ((3 || 4 || 5) && (6 && 7)) && 8
> [1, &&, 2] -> [||] -> [ [3, ||, 4, ||, 5] -> [&&] -> [6, &&, 7] ] -> [&&] -> [8]
```
			 ||
		/			\
	1 && 2				&&
					/		\
				&&			8
			/		\
	3 || 4 || 5		6 && 7

On peut eventuellement envisager ça pour les types:
- 0 : simple command
- 1 : compound command
- 2 : metacaracter

control operator
A token that performs a control function. It is a newline or one of the following: ‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.

Et le tralala que Karimo a déjà fait
Attention, quand on lancera l'appel pour le subshell dans l'étape du dessus, il faudra faire ça aussi ! (i think)
- Performs the various shell expansions (see Shell Expansions), breaking the expanded tokens into lists of filenames (see Filename Expansion) and commands and arguments.
```
#chatjpp
Perform variable expansion:
The shell performs variable expansion for the environment variables used in the command ($INPUT_FILE, $OUTPUT_FILE, $LOG_FILE), replacing them with their respective values.
```
Redirection time, comme ce minishell qui me donne envie de me rediriger en cap patisserie+plomberie:
- Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list.

Ici on fait nos petits dup2 j'imagine, on prend les piti fichier et on remplace les piti files descriptor par les piti fichier <3 trop kawaii trop grr

- Executes the command (see Executing Commands).
Optionally waits for the command to complete and collects its exit status (see Exit Status).

- dans export, verifier que chaque variable qu'on rajoute respecte la specification posix:
environment variable names must consist solely of uppercase letters, digits, and underscores (_), and they must not start with a digit.

EXECVE OH OUI ENFIN AH OUI OUI OUI OUI OUI

puis on gère les signaux <3


# Redirections

tree->left = reponse()
operation
tree->right = reponse()

if (node == '&&')
{
	if (reponse_gauche == 0): //succes
		regarder reponse_droite
	else if (reponse_gauche == 1): //failure
		return ;
}

if (node == '||')
{
	if (reponse_gauche == 0) //success
		c bon
	sinon
		regarder reponse_droite
}

- >
if (node == '>')
{
	reponse_gauche -> stdout -> enlever tt du fichier et rajouter la sortie de gauche
}
- <
if (node == '<')
{
	PROBLEMATIQUE AYAYAYAYA
}
- >>
if (node == '>>')
{
	reponse_gauche -> stdout -> append to fichier a droite, si pas de fichier on en cree un nv
}
- <<

operation
tree->left = reponse()
tree->right = reponse()

int	exec_rec(root, args??, type_op):
	if (!root->left && !root->right):
		if (type_op == >, <):
			
		return (execution(root->content))
	if (root->content == &&):
		rep_left = exec_rec(root->left, NULL)
		if (rep_left == FAILURE):
			return exit status FAILURE // on finit l'execution
		rep_right = exec_rec(root>right, NULL)
	if (root->content == ||):
		rep_left = exec_rec(root->left, NULL)
		if (rep_left == SUCCES):
			return exit status SUCCES // on finit l'execution
		rep_right = exec_rec(root>right, NULL)
	if (root->content == |):
		rep_left = exec_rec(root->left, NULL) // recuperer la valeur renvoyee par le fils gauche et la donner au fils droite
		rep_right = exec_rec(root->right, rep_left)
	if (root->content == > || root->content == <):
		if (root->right->content == is file)
			char *fichier = root->right->content
			OPEN(fichier)
		rep_right_fichier = exec_rec(root->right, )
		rep_left = exec_rec(root->left, rep_right_fichier, root->content)
	if (root->content == <<): //here_doc
		char *LIMITER = root->right->content
		rep_left = exec_rec(root->left, fichier)
	






