- Mémo des cas tordus
CDPATH
Command
$> export CDPATH=/
$> cd /home
$> ls bin
ls: cannot access 'bin': No such file or directory
$> cd bin

Expected
/bin
$> pwd
/bin

Empty output redirection to file
Command
$> rm -f pouic
$> >pouic
$> ls -l pouic

Expected
-rw-r--r-- 1 jodufour user42 0  3 déc.  16:19 pouic

Broken pipe
Command
$> rm -f pouic
$> echo koala >pouic |

Expected
Any handle but no crash

Redirection priority
Command
$> rm -f pouic
$> rm pouic > pouic
$> ls -l pouic

Expected
ls: cannot access 'pouic': No such file or directory

Empty output redirection to pipe to file
Command
$> rm -f pouic
$> >|pouic

Expected
bash: syntax error near unexpected token `|'


Quand j’exécute `> | file` j’ai un message d’erreur et le fichier n’est pas créé
Token merging
Command-0
$> echo koala > pouic
$> cat"-e" pouic
bash: cat-e: command not found

Expected-0
bash: cat-e: command not found

Command-1
$> e'c'"h"o koala>pouic
$> cat 'po'"u"ic

Expected-1
koala

Options are considered as arguments
Command
$> export OPT=-n
$> "echo $OPT" tutu

Expected
bash: echo -n: command not found

Consecutive pipes
Command
$> echo koala|||cat

Expected
bash: syntax error near unexpected token `|'

Forcing consider arguments AND stdin
Command
$> echo lolilol > pouic
$> echo tutu > poneyvif
$> cat < pouic poneyvif -

Expected
tutu
lolilol

Expanding unseparated env variables
Command
$> export TUTU=koala FOO=google
$> echo $TUTU$FOO

Expected
koalagoogle

Expanding the right variables
Command
$> export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8
$> echo "$A'$B"'$C"$D'$E'"$F"'"'$G'$H"

Expected
1'2$C"$D5"$F"'7'8


No expanding when $ has no valid name behind
Command-0
$> echo " $ " | cat -e

Expected-0
 $ $

Command-1
$> echo $:$= | cat -e

Expected-1
$:$=$

Remove $ when last character of an unquoted portion followed by a quoted portion
Command
$> echo foo$"HOME"

Expected
fooHOME

Detect commands that are placed AFTER the redirections
Command
$> echo koala>pouic
$> <pouic cat
koala

Expected
koala

Detect quotes that result of an expanding
Command
$> export FOO='"'
$> echo " $FOO " | cat -e

Expected
 " $


Multiple commands execution
Command
$> echo koala > pouic
echo titi > foo
echo tutu > bar
cat pouic foo bar

Expected
koala
titi
tutu

Split one token into N + 1 tokens when an unquoted expand contains N spaces
Command

$> mkdir dir_test
$> cd dir_test
$> touch tutu
$> echo koala > titi
$> export FOO='s -ls'
$> l$FOO

Expected
total 12
drwxr-xr-x  2 jodufour user42 4096  9 déc.  23:43 .
drwxr-xr-x 10 jodufour user42 4096  9 déc.  23:42 ..
-rw-r--r--  1 jodufour user42	6  9 déc.  23:43 titi
-rw-r--r--  1 jodufour user42	0  9 déc.  23:42 tutu

Do not expand here-doc’s delimiter
Consider only the last output redirection as effective
Command
$> rm -f out0 out1
$> echo tutu>out0>out1
$> ls -l out0 out1

Expected
-rw-r--r-- 1 jodufour user42 0 15 déc.  20:52 out0
-rw-r--r-- 1 jodufour user42 5 15 déc.  20:52 out1


Do not exit the shell if the exit command is piped or piping
Command-0
$> echo koala > pouic
$> exit | cat -e pouic

Expected-0
koala$

Command-1
$> echo koala > pouic
$> cat -e pouic | exit

Expected-1
(nothing)

Input redirections override pipe redirections
Command
$> echo koala>foo
$> echo pouic>bar
$> cat foo|cat<bar

Expected
pouic


Expand here-doc content ONLY when  delimiter is unquoted
Command-0
$> export TUTU='hello world'
$> cat << stop -e
> koala
> '$TUTU'
> $TUTU
> "$TUTU"
> stop

Expected-0
koala$
'hello world'$
hello world$
"hello world"$

Command-1
$> export TUTU='this is me'
$> cat << "eof" -e
> milou
> '$TUTU'
> $TUTU
> "$TUTU"
> eof

Expected-1
milou$
'$TUTU'$
$TUTU$
"$TUTU"$



