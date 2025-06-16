#include <cstdio>
#include <cstdlib>
#include <algorithm>

class AVLTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;
        int count;

        explicit Node(int k) : key(k), left(nullptr), right(nullptr), height(1), count(1) {}
    };

    Node* root;

    int GetHeight(Node* node) {
        return node ? node->height : 0;
    }

    int GetBalance(Node* node) {
        return node ? GetHeight(node->left) - GetHeight(node->right) : 0;
    }

    void UpdateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
        }
    }

    Node* RotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        UpdateHeight(y);
        UpdateHeight(x);

        return x;
    }

    Node* RotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        UpdateHeight(x);
        UpdateHeight(y);

        return y;
    }

    Node* Insert(Node* node, int key) {
        if (!node) return new Node(key);

        if (key < node->key) {
            node->left = Insert(node->left, key);
        } else if (key > node->key) {
            node->right = Insert(node->right, key);
        } else {
            return node;  // Duplicate, do nothing
        }

        UpdateHeight(node);

        int balance = GetBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key) {
            return RotateRight(node);
        }

        // Right Right Case
        if (balance < -1 && key > node->right->key) {
            return RotateLeft(node);
        }

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = RotateLeft(node->left);
            return RotateRight(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = RotateRight(node->right);
            return RotateLeft(node);
        }

        return node;
    }

    Node* FindMin(Node* node) {
        Node* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    int FindNext(Node* node, int key) {
        Node* current = node;
        Node* successor = nullptr;

        while (current) {
            if (current->key >= key) {
                successor = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return successor ? successor->key : -1;
    }

    void ClearTree(Node* node) {
        if (node) {
            ClearTree(node->left);
            ClearTree(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        ClearTree(root);
    }

    void Add(int key) {
        root = Insert(root, key);
    }

    int Next(int key) {
        return FindNext(root, key);
    }
};

int main() {
    AVLTree tree;
    int n;
    std::scanf("%d", &n);

    char op;
    int val;
    int last_query_result = 0;
    bool last_was_query = false;

    for (int i = 0; i < n; ++i) {
        std::scanf(" %c %d", &op, &val);

        if (op == '+') {
            if (last_was_query) {
                val = (val + last_query_result) % 1000000000;
            }
            tree.Add(val);
            last_was_query = false;
        } else if (op == '?') {
            last_query_result = tree.Next(val);
            std::printf("%d\n", last_query_result);
            last_was_query = true;
        }
    }

    return 0;
}