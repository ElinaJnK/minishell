#!/bin/env python3

from subprocess import run, PIPE, DEVNULL, CompletedProcess
import sys

class colors:
    DEFAULT = '\033[39m'
    RED = '\033[31m'
    GREEN = '\033[32m'
    MEGENTA = '\033[33m'

def compare_output(minishell: CompletedProcess, bash: CompletedProcess):
	if minishell.stdout == bash.stdout:
		print(f"{colors.GREEN}output: OK", end="	")
	else:
		print(f"{colors.RED}output: FAILED", end="	")
	if minishell.returncode == bash.returncode:
		print(f"{colors.GREEN}exit status: OK{colors.DEFAULT}\n")
	else:
		print(f"{colors.RED}exit status: FAILED{colors.DEFAULT}\n")

if len(sys.argv) == 1:
	while True:
		line = input("> ")
		if line == "":
			break
		nline = line.replace("; ", "\n")
		minishell = run(["./minishell"], stdout=PIPE, stderr=DEVNULL, input=f"{nline}\n", encoding="ascii")
		bash = run(["bash", "--posix"], stdout=PIPE, stderr=DEVNULL, input=f"{nline}\n", encoding="ascii")
		print(colors.MEGENTA + line)
		compare_output(minishell, bash)
else:
	with open(sys.argv[1], 'r') as tests:
		for line in tests.read().split("\n"):
			if line == "" or line[0] == "#":
				continue
			nline = line.replace("; ", "\n")
			minishell = run(["./minishell"], stdout=PIPE, stderr=DEVNULL, input=f"{nline}\n", encoding="ascii")
			bash = run(["bash", "--posix"], stdout=PIPE, stderr=DEVNULL, input=f"{nline}\n", encoding="ascii")
			print(colors.MEGENTA + line)
			compare_output(minishell, bash)