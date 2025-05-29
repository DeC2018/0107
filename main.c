#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 2000

// Definition for a binary tree node.
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Helper to create a new tree node
struct TreeNode* newNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Helper to parse input string like "[3,9,20,null,null,15,7]" to array
int parseInput(const char* s, char arr[][16], int* n) {
    int i = 0, j = 0, k = 0;
    while (s[i] && (s[i] == ' ' || s[i] == '[')) ++i;
    while (s[i] && s[i] != ']') {
        if (s[i] == ',') {
            arr[k][j] = '\0';
            ++k; j = 0;
        } else if (s[i] != ' ') {
            arr[k][j++] = s[i];
        }
        ++i;
    }
    if (j > 0) {
        arr[k][j] = '\0';
        ++k;
    }
    *n = k;
    return k;
}

// Helper to build tree from level-order array
struct TreeNode* buildTree(char arr[][16], int n) {
    if (n == 0 || strcmp(arr[0], "null") == 0) return NULL;
    struct TreeNode** nodes = (struct TreeNode**)malloc(n * sizeof(struct TreeNode*));
    for (int i = 0; i < n; ++i) nodes[i] = NULL;
    struct TreeNode* root = newNode(atoi(arr[0]));
    nodes[0] = root;
    int idx = 1;
    for (int i = 0; i < n && idx < n; ++i) {
        if (!nodes[i]) continue;
        // left child
        if (idx < n && strcmp(arr[idx], "null") != 0) {
            nodes[i]->left = newNode(atoi(arr[idx]));
            nodes[idx] = nodes[i]->left;
        }
        idx++;
        // right child
        if (idx < n && strcmp(arr[idx], "null") != 0) {
            nodes[i]->right = newNode(atoi(arr[idx]));
            nodes[idx] = nodes[i]->right;
        }
        idx++;
    }
    free(nodes);
    return root;
}

// Helper to free the memory of a tree
void deleteTree(struct TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

// Bottom-up level order traversal
int** levelOrderBottom(struct TreeNode* root, int* returnSize, int** returnColumnSizes){
    if (!root) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    struct TreeNode* queue[MAX_NODES];
    int front = 0, rear = 0;
    queue[rear++] = root;

    int** levels = malloc(sizeof(int*) * MAX_NODES);
    *returnColumnSizes = malloc(sizeof(int) * MAX_NODES);
    int levelIndex = 0;

    while (front < rear) {
        int size = rear - front;
        levels[levelIndex] = malloc(sizeof(int) * size);
        (*returnColumnSizes)[levelIndex] = size;

        for (int i = 0; i < size; ++i) {
            struct TreeNode* node = queue[front++];
            levels[levelIndex][i] = node->val;
            if (node->left) queue[rear++] = node->left;
            if (node->right) queue[rear++] = node->right;
        }
        levelIndex++;
    }

    // Reverse the levels array
    int** result = malloc(sizeof(int*) * levelIndex);
    int* columnSizes = malloc(sizeof(int) * levelIndex);
    for (int i = 0; i < levelIndex; ++i) {
        result[i] = levels[levelIndex - 1 - i];
        columnSizes[i] = (*returnColumnSizes)[levelIndex - 1 - i];
    }

    free(*returnColumnSizes);
    *returnColumnSizes = columnSizes;
    *returnSize = levelIndex;

    free(levels);  // Only free the top-level pointer, not the rows

    return result;
}

// Helper to print the bottom-up level order traversal result
void printResult(int** result, int returnSize, int* returnColumnSizes) {
    printf("[");
    for (int i = 0; i < returnSize; ++i) {
        printf("[");
        for (int j = 0; j < returnColumnSizes[i]; ++j) {
            printf("%d", result[i][j]);
            if (j + 1 < returnColumnSizes[i])
                printf(",");
        }
        printf("]");
        if (i + 1 < returnSize)
            printf(",");
    }
    printf("]\n");
}

int main() {
    const char* inputs[] = {
        "[3,9,20,null,null,15,7]",
        "[1]",
        "[]"
    };
    int num_cases = sizeof(inputs) / sizeof(inputs[0]);

    for (int i = 0; i < num_cases; ++i) {
        printf("Input: root = %s\n", inputs[i]);
        char arr[MAX_NODES][16];
        int n = 0;
        parseInput(inputs[i], arr, &n);
        struct TreeNode* root = buildTree(arr, n);

        int returnSize = 0;
        int* returnColumnSizes = NULL;
        int** result = levelOrderBottom(root, &returnSize, &returnColumnSizes);

        printf("Output: ");
        if (result)
            printResult(result, returnSize, returnColumnSizes);
        else
            printf("[]\n");

        for (int j = 0; j < returnSize; ++j)
            free(result[j]);
        free(result);
        free(returnColumnSizes);

        deleteTree(root);
    }
    return 0;
}
