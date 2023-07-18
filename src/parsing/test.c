#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ASTNode {
	char* command;
	struct ASTNode* left;
	struct ASTNode* right;
} ASTNode;

ASTNode* createNode(char* command) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->command = command;
	node->left = NULL;
	node->right = NULL;
	return node;
}

ASTNode* buildAST(char** tokens, int start, int end) {
	if (start > end)
		return NULL;

	ASTNode* root = NULL;
	ASTNode* current = NULL;
	int i = start;

	while (i <= end) {
		if (!root)
			root = current;
		if (strcmp(tokens[i], "(") == 0) {
			int openCount = 1;
			int closeCount = 0;
			int j = i + 1;

			while (j <= end) {
				if (strcmp(tokens[j], "(") == 0)
					openCount++;
				else if (strcmp(tokens[j], ")") == 0)
					closeCount++;

				if (openCount == closeCount)
					break;

				j++;
			}
			ASTNode* subtree = buildAST(tokens, i + 1, j - 1);
			if (current == NULL) {
				root = subtree;
			} else {
				current->right = subtree;
				// if (!root)
				// 	root = current;
			}
			i = j;
		} else if (strcmp(tokens[i], "||") == 0 || strcmp(tokens[i], "&&") == 0 || strcmp(tokens[i], "|") == 0
				)
		{
			ASTNode* node = createNode(tokens[i]);
			node->left = root;
			current = node;
			root = NULL;
		}
		else if (strcmp(tokens[i], "<") == 0 || strcmp(tokens[i], ">") == 0)
		{
			ASTNode* node = createNode(tokens[i]);
			if (current == NULL)
			{
				node->left = root;
				current = node;
				root = node;
			}
			else
			{
				ASTNode* tmp = current;
				while (tmp && tmp->right)
					tmp = tmp->right;
				current->right = node;
				node->left = tmp;
				current = node;
			}
		}
		else
		{
			ASTNode* node = createNode(tokens[i]);
			if (current == NULL) {
				root = node;
			} else {
				current->right = node;
			}
		}
		i++;
	}
	if (root == NULL)
		root = current;
	return root;
}

// Function to tokenize the command string
char** tokenizeCommand(char* command, int* count) {
	char* token;
	char** tokens = NULL;
	int i = 0;

	token = strtok(command, " ");
	while (token != NULL) {
		tokens = realloc(tokens, sizeof(char*) * (i + 1));
		tokens[i] = token;
		i++;

		token = strtok(NULL, " ");
	}

	*count = i;
	return tokens;
}

// Function to recursively free the memory of the AST
void freeAST(ASTNode* node) {
	if (node == NULL)
		return;
	freeAST(node->left);
	freeAST(node->right);
	free(node);
}

// Function to print the AST in postfix order (for demonstration purposes)
void printASTb(ASTNode* node) {
	if (node == NULL)
		return;
	printASTb(node->left);
	printf("%s ", node->command);
	printASTb(node->right);
}

void printSpaces(int count) {
    for (int i = 0; i < count; i++)
        printf(" ");
}

void printASTHelper(ASTNode* node, int depth, int isRight) {
	if (node == NULL)
		return;

    int INDENTATION_SIZE = 4;
    depth += INDENTATION_SIZE;

    printASTHelper(node->right, depth, 1);
    printSpaces(depth - INDENTATION_SIZE);

    if (isRight)
        printf("┌─");
    else
        printf("└─");

    printf("%s\n", node->command);
	
    printASTHelper(node->left, depth, 0);
}

void printAST(ASTNode* root) {
    printASTHelper(root, 0, 0);
}

int main() {
	// char command[] = "cmd1 || ( cmd2 && ( cmd3 || cmd4 ) )";
	//char command[] = "( 1 && 2 ) || ( ( 3 || 4 || 5 ) && ( 6 || 7 ) ) && 8";
	//char command[] = "cmd3 || cmd4 && cmd5";
	//char command[] = "cmd1 || ( cmd3 || cmd4 ) && cmd5";
	//char command[] = "( cmd1 && cmd2 ) > f2";
	char command[] = "( cmd1 > e && cmd2 && cmd3 > d ) > f3";
	printf("cmd: %s\n", command);
	int count;
	char** tokens = tokenizeCommand(command, &count);
	int i = 0;
	while(tokens && i < count)
	{
		printf("%s\n", tokens[i]);
		i++;
	}
	printf("count: %d\n", count);
	ASTNode* root = buildAST(tokens, 0, count - 1);
	printAST(root);
	printf("\n");

	// Cleanup: Free the memory of the AST and tokens
	freeAST(root);
	free(tokens);
	return 0;
}
