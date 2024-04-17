#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256
#define MAX_PATTERNS 1000
#define MAX_PATTERN_LENGTH 100

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    struct TrieNode *failureLink;
    int output;
};

struct TrieNode *createNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    node->failureLink = NULL;
    node->output = 0;
    return node;
}

void insertPattern(struct TrieNode *root, char *pattern) {
    struct TrieNode *current = root;
    for (int i = 0; i < strlen(pattern); i++) {
        int index = (int)pattern[i];
        if (current->children[index] == NULL) {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->output = 1; 
}

void buildFailureLinks(struct TrieNode *root) {
    struct TrieNode *queue[MAX_PATTERNS]; 
    int front = 0, rear = 0;
    queue[rear++] = root;
    
    while (front < rear) {
        struct TrieNode *current = queue[front++];
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i] != NULL) {
                struct TrieNode *temp = current->failureLink;
                while (temp != NULL && temp->children[i] == NULL) {
                    temp = temp->failureLink;
                }
                if (temp != NULL) {
                    current->children[i]->failureLink = temp->children[i];
                } else {
                    current->children[i]->failureLink = root;
                }
                queue[rear++] = current->children[i];
            }
        }
    }
}
void searchPatterns(struct TrieNode *root, char *text) {
    struct TrieNode *current = root;
    for (int i = 0; i < strlen(text); i++) {
        int index = (int)text[i];
        while (current->children[index] == NULL && current != root) {
            current = current->failureLink;
        }
        if (current->children[index]) {
            current = current->children[index];
            if (current->output) {
                printf("Pattern found at position: %d\n", i - strlen(text) + strlen(text) + 1);
            }
        }
    }
}

int main() {
    struct TrieNode *root = createNode();

    char patternsFileName[MAX_PATTERN_LENGTH];
    char textFileName[MAX_PATTERN_LENGTH];
    printf("Enter the name of the file containing patterns: ");
    scanf("%s", patternsFileName);

    FILE *patternFile = fopen(patternsFileName, "r");
    if (patternFile == NULL) {
        perror("Error opening patterns file");
        return 1;
    }
    char pattern[MAX_PATTERN_LENGTH];
    while (fgets(pattern, MAX_PATTERN_LENGTH, patternFile) != NULL) {
        if (pattern[strlen(pattern) - 1] == '\n') {
            pattern[strlen(pattern) - 1] = '\0';
        }
        insertPattern(root, pattern);
    }
    fclose(patternFile);

    buildFailureLinks(root);

    printf("Enter the name of the file containing text: ");
    scanf("%s", textFileName);

    FILE *textFile = fopen(textFileName, "r");
    if (textFile == NULL) {
        perror("Error opening text file");
        return 1;
    }
    char text[MAX_PATTERN_LENGTH];
    if (fgets(text, MAX_PATTERN_LENGTH, textFile) != NULL) {
        if (text[strlen(text) - 1] == '\n') {
            text[strlen(text) - 1] = '\0';
        }
        searchPatterns(root, text);
    }
    fclose(textFile);

    return 0;
}
