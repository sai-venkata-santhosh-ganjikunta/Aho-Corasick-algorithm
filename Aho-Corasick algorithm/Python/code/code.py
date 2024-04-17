from queue import Queue

ALPHABET_SIZE = 256
MAX_PATTERNS = 1000
MAX_PATTERN_LENGTH = 100

class TrieNode:
    def __init__(self):
        self.children = [None] * ALPHABET_SIZE
        self.failureLink = None
        self.output = 0

def insertPattern(root, pattern):
    current = root
    for c in pattern:
        index = ord(c)
        if not current.children[index]:
            current.children[index] = TrieNode()
        current = current.children[index]
    current.output = 1

def buildFailureLinks(root):
    queue = Queue()
    queue.put(root)

    while not queue.empty():
        current = queue.get()
        for i in range(ALPHABET_SIZE):
            if current.children[i]:
                temp = current.failureLink
                while temp and not temp.children[i]:
                    temp = temp.failureLink
                if temp:
                    current.children[i].failureLink = temp.children[i]
                else:
                    current.children[i].failureLink = root
                queue.put(current.children[i])

def searchPatterns(root, text):
    current = root
    for i in range(len(text)):
        index = ord(text[i])
        while current != root and not current.children[index]:
            current = current.failureLink
        if current.children[index]:
            current = current.children[index]
            if current.output:
                print("Pattern found at position:", i - len(text) + len(text) + 1)

if __name__ == "__main__":
    root = TrieNode()

    patternsFileName = input("Enter the name of the file containing patterns: ")

    with open(patternsFileName, 'r') as patternFile:
        for pattern in patternFile:
            insertPattern(root, pattern.strip())

    buildFailureLinks(root)

    textFileName = input("Enter the name of the file containing text: ")

    with open(textFileName, 'r') as textFile:
        text = textFile.readline()
        searchPatterns(root, text)
