#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *tagName;
    char **attributes;
    struct Node **children;
    char *textContent;
    int attrCount;
    int childCount;
} Node;

Node* createNode(char *tagName) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->tagName = _strdup(tagName);
    node->attributes = NULL;
    node->children = NULL;
    node->textContent = NULL;
    node->attrCount = 0;
    node->childCount = 0;
    return node;
}

void addAttribute(Node *node, char *attrName, char *attrValue) {
    node->attributes = (char**) realloc(node->attributes, (node->attrCount + 1) * 2 * sizeof(char*));
    node->attributes[node->attrCount * 2] = _strdup(attrName);
    node->attributes[node->attrCount * 2 + 1] = _strdup(attrValue);
    node->attrCount++;
}

void addChild(Node *parent, Node *child) {
    parent->children = (Node**) realloc(parent->children, (parent->childCount + 1) * sizeof(Node*));
    parent->children[parent->childCount] = child;
    parent->childCount++;
}

void setTextContent(Node *node, char *textContent) {
    node->textContent = _strdup(textContent);
}

Node* parseElement(FILE *file) {
    char line[256], tagName[256], attrName[256], attrValue[256], textContent[256];
    Node *currentNode = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, " <%s", tagName) == 1) {
            currentNode = createNode(tagName);
            while (fgets(line, sizeof(line), file) && sscanf(line, " %s = \"%[^\"]\"", attrName, attrValue) == 2) {
                addAttribute(currentNode, attrName, attrValue);
            }
            if (fgets(line, sizeof(line), file) && sscanf(line, " %[^<]", textContent) == 1) {
                setTextContent(currentNode, textContent);
            }
            return currentNode;
        }
    }
    return NULL;
}

Node* parseXML(char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    Node *root = parseElement(file);
    fclose(file);
    return root;
}

void searchAndPrint(Node *node, const char *searchTag, const char *searchAttr) {
    if (strcmp(node->tagName, searchTag) == 0) {
        printf("Found tag: %s\n", node->tagName);
        for (int i = 0; i < node->attrCount; i++) {
            if (strcmp(node->attributes[i * 2], searchAttr) == 0) {
                printf("Attribute: %s, Value: %s\n", node->attributes[i * 2], node->attributes[i * 2 + 1]);
            }
        }
        if (node->textContent) {
            printf("Text Content: %s\n", node->textContent);
        }
    }
    for (int i = 0; i < node->childCount; i++) {
        searchAndPrint(node->children[i], searchTag, searchAttr);
    }
}

int main() {
    Node *root = parseXML("path/to/your/file.xml");
    if (root) {
        searchAndPrint(root, "targetTag", "targetAttr");
        // Free memory (not shown for brevity)
    }
    return 0;
}
