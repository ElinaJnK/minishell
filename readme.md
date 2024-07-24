# README - minishell
## _With Karima Sadykova (aka Karimo)_

_Special thanks to Samy Naji for the numerous tests (including test_commands.py) and Akadil Kalimoldayev for the pipe._

Summary of all the steps to take for minishell. This README is not final and may contain ideas that we later changed. For example, the tokenization part took much longer than initially expected. Please check the code and contact us if you find a leak :)

First, here are the instructions provided by bash:

_The following is a brief description of the shell’s operation when it reads and executes a command. Basically, the shell does the following:_
- _Reads its input from a file (see Shell Scripts), from a string supplied as an argument to the -c invocation option (see Invoking Bash), or from the user’s terminal._
- _Breaks the input into words and operators, obeying the quoting rules described in Quoting. These tokens are separated by metacharacters. Alias expansion is performed by this step (see Aliases)._
- _Parses the tokens into simple and compound commands (see Shell Commands)._
- _Performs the various shell expansions (see Shell Expansions), breaking the expanded tokens into lists of filenames (see Filename Expansion) and commands and arguments._
- _Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list._
- _Executes the command (see Executing Commands). Optionally waits for the command to complete and collects its exit status (see Exit Status)._

Let’s start with this:

- _Reads its input from a file (see Shell Scripts), from a string supplied as an argument to the -c invocation option (see Invoking Bash), or from the user’s terminal._

A check of previous projects indicates that ./script.sh should not execute what is in the script. Therefore, we can plan for a function:
```c
read_stdin()
```
which will simply read the entire line.

Alright, now for some fun:
- _Breaks the input into words and operators, obeying the quoting rules described in Quoting. These tokens are separated by metacharacters. Alias expansion is performed by this step (see Aliases)._

Here is the plan we decided to follow initially:
```
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
```
This plan is a good base line but some parts needed refining and some special test cases made us modify steps.
We decided to use a linked list:
```c
typedef struct s_token
{
    char            *content;
    int             type;
    struct s_token  *next;
}       t_token;
```
to have something that looks like this:
```sh
> ec'ho' nana && (cd folder || echo yum)
# gives
> [echo, nana, &&, (, cd, folder, ||, echo, yum, )]
```
basically, each thing separated by a space is an element of the list.

The goal was to take the entire string and call a function that will return the string analyzed without quotes or apostrophes.
After that, tokenize and create a linked list of type t_token.
We also created a function check_com whose goal was to analyze if there was any mistake detected in the syntax at this point.

There were also some specific cases to handle like:
```
ech"o y"n
bash: echo yn: command not found
```
the space is not the separator here! Indeed, between quotes, we need to take everything and once out of quotes, we resume normal behavior. Also, watch out for this:
```
ech"o " yn
```
this doesn't work because we are sending echo[space] to bash which is not a valid command.

Now we have each element in our list with their type, which we identified simply with if statments (switch/case was not allowed in the subject of our project):
```c
int is_op(char *line, int i)
{
    if (ft_strncmp(line + i, "&&", 2) == 0)
        return (1);
    else if (ft_strncmp(line + i, "||", 2) == 0)
        return (2);
    return (0);
}

int is_fb(char *line, int i)
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

P.S.: Before moving to the next part, remember to handle here_doc! It’s easier to do it in the parsing phase than during execution.

What’s next?

- _Parses the tokens into simple and compound commands (see Shell Commands)._
Here we watch. Here we analyze, here we look for: parentheses! Initially, we thought of creating a new list that would separate compound commands (within parentheses) and simple commands (like echo a for example). However, we quickly realized that this was quite difficult to manage. So, we created binary trees :D.

The algo's operation is quite simple:
- Check the type of metacharacter: <, <<, >, >>, &&, || or (, )
- If there is a parenthesis, find the last parenthesis and make a recursive call on this new command segment
- If there are parentheses within parentheses, make another recursive call
- These recursive calls will create "subtrees" that will then be added to the main tree.

This code can be found in /parsing/build_ast.c and /utils/ast_utils.c. Ultimately, for this command:
```sh
> (1 && 2) || ((3 || 4 || 5) && (6 && 7)) && 8
> [1, &&, 2] -> [||] -> [ [3, ||, 4, ||, 5] -> [&&] -> [6, &&, 7] ] -> [&&] -> [8]
```
We’ll have a tree that looks like this:
```
    ||
   /  \
  &&    &&
 /  \   / \
1    2 ||   &&
     / | \ / \
    3  4  5 6  7
```

Next, we perform shell expansions early during tokenization. This fit much better with our code so far (and it’s not very practical to do it in a tree).

Redirection time, like this minishell makes me want to redirect myself to a pastry+plumbing career:
- _Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list._

Here we do our little dup2, we take the little files and replace the little file descriptors with the little files <3

- _Executes the command (see Executing Commands). Optionally waits for the command to complete and collects its exit status (see Exit Status)._ 

This is in /exec/exec_ast.c. Just as we built the tree, we now traverse it and apply different functions according to different nodes, and finally execute the leaves.

We also handle builtins at the leaf level, with some checks:
- In export, ensure each added variable respects POSIX specifications:
environment variable names must consist solely of uppercase letters, digits, and underscores (_), and they must not start with a digit.

Finally, we handled signals, the file is in /shell/signal.c.

At the end of the project, we will also include some tests that seem a bit counterintuitive (update: they are now in tests.txt :)).

### Run the program

Simply do:
```bash
make
./minishell
```
And type commands !

### Additional Notes for Testing
For testing, we used a Python script named `test_commands.py`. To ensure proper functionality with readline, add the following line to the script:
```python
rl_outstream = stderr
```
and then execute as you would your usual python script.
```python
python3 test_commands.py
```
If you have additional tests, we would be happy to add them to our small database.
	
