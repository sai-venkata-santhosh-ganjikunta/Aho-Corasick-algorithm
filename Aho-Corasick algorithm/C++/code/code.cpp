#include <iostream>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

#define ALPHABET_SIZE 256
#define MAX_PATTERNS 1000
#define MAX_PATTERN_LENGTH 100

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    TrieNode* failureLink;
    int output;

    TrieNode() : failureLink(nullptr), output(0) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            children[i] = nullptr;
        }
    }
};

void insertPattern(TrieNode* root, const string& pattern) {
    TrieNode* current = root;
    for (char c : pattern) {
        int index = static_cast<int>(c);
        if (!current->children[index]) {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }
    current->output = 1; 
}

void buildFailureLinks(TrieNode* root) {
    std::queue<TrieNode*> queue;
    queue.push(root);

    while (!queue.empty()) {
        TrieNode* current = queue.front();
        queue.pop();

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (current->children[i]) {
                TrieNode* temp = current->failureLink;
                while (temp && !temp->children[i]) {
                    temp = temp->failureLink;
                }
                if (temp) {
                    current->children[i]->failureLink = temp->children[i];
                } else {
                    current->children[i]->failureLink = root;
                }
                queue.push(current->children[i]);
            }
        }
    }
}

void searchPatterns(TrieNode* root, const string& text) {
    TrieNode* current = root;
    for (size_t i = 0; i < text.length(); ++i) {
        int index = static_cast<int>(text[i]);
        while (current != root && !current->children[index]) {
            current = current->failureLink;
        }
        if (current->children[index]) {
            current = current->children[index];
            if (current->output) {
                cout << "Pattern found at position: " << i - text.length() + text.length() + 1 << endl;
            }
        }
    }
}

int main() {
    TrieNode* root = new TrieNode();

    string patternsFileName;
    string textFileName;

    cout << "Enter the name of the file containing patterns: ";
    cin >> patternsFileName;

    ifstream patternFile(patternsFileName);
    if (!patternFile.is_open()) {
        cerr << "Error opening patterns file" << endl;
        return 1;
    }

    string pattern;
    while (getline(patternFile, pattern)) {
        insertPattern(root, pattern);
    }
    patternFile.close();

    buildFailureLinks(root);

    cout << "Enter the name of the file containing text: ";
    cin >> textFileName;

    ifstream textFile(textFileName);
    if (!textFile.is_open()) {
        cerr << "Error opening text file" << endl;
        return 1;
    }

    string text;
    if (getline(textFile, text)) {
        searchPatterns(root, text);
    }
    textFile.close();

    delete root;

    return 0;
}
