// Binary Search Tree in C++17 (no set or map used)
// Compile: g++ -std=c++17 -Wall -Wextra -o bst bst.cpp

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ---------- Node ----------
struct Node {
    int key;
    Node* left;
    Node* right;

    explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// ---------- BinarySearchTree ----------
class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroy(root);
    }

    // Don't allow copying (raw pointers would cause a double delete)
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    void insert(int key) {
        root = insert(root, key);
    }

    bool search(int key) const {
        return search(root, key);
    }

    void remove(int key) {
        root = remove(root, key);
    }

    // Traversals return a vector so they're easy to print and check
    vector<int> inorder() const {
        vector<int> out;
        inorder(root, out);
        return out;
    }

    vector<int> preorder() const {
        vector<int> out;
        preorder(root, out);
        return out;
    }

    vector<int> postorder() const {
        vector<int> out;
        postorder(root, out);
        return out;
    }

    // Height = number of nodes on the longest root-to-leaf path.
    // An empty tree has height 0, a single node has height 1.
    int height() const {
        return height(root);
    }

private:
    Node* root;

    // Insert: go left if smaller, right if larger, ignore duplicates.
    Node* insert(Node* node, int key) {
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        // else: key already exists, do nothing

        return node;
    }

    bool search(const Node* node, int key) const {
        if (node == nullptr)
            return false;
        if (key == node->key)
            return true;
        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    // Smallest node in a subtree = keep going left
    Node* findMin(Node* node) const {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node* remove(Node* node, int key) {
        if (node == nullptr)
            return nullptr; // key not found

        if (key < node->key) {
            node->left = remove(node->left, key);
        } else if (key > node->key) {
            node->right = remove(node->right, key);
        } else {
            // Found the node to delete

            // Case 1: leaf (no children)
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            // Case 2: one child -> replace node with that child
            if (node->left == nullptr) {
                Node* child = node->right;
                delete node;
                return child;
            }
            if (node->right == nullptr) {
                Node* child = node->left;
                delete node;
                return child;
            }

            // Case 3: two children
            // Copy the inorder successor (smallest key in the right subtree)
            // into this node, then delete the successor from the right subtree.
            Node* successor = findMin(node->right);
            node->key = successor->key;
            node->right = remove(node->right, successor->key);
        }
        return node;
    }

    void inorder(const Node* node, vector<int>& out) const {
        if (node == nullptr)
            return;
        inorder(node->left, out);  // left
        out.push_back(node->key);  // root
        inorder(node->right, out); // right
    }

    void preorder(const Node* node, vector<int>& out) const {
        if (node == nullptr)
            return;
        out.push_back(node->key);   // root
        preorder(node->left, out);  // left
        preorder(node->right, out); // right
    }

    void postorder(const Node* node, vector<int>& out) const {
        if (node == nullptr)
            return;
        postorder(node->left, out);  // left
        postorder(node->right, out); // right
        out.push_back(node->key);    // root
    }

    int height(const Node* node) const {
        if (node == nullptr)
            return 0;
        int lh = height(node->left);
        int rh = height(node->right);
        return 1 + (lh > rh ? lh : rh);
    }

    void destroy(Node* node) {
        if (node == nullptr)
            return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};

// ---------- Helper functions for the demo ----------
void printVector(const string& label, const vector<int>& v) {
    cout << label;
    for (int x : v)
        cout << x << " ";
    cout << "\n";
}

bool isSorted(const vector<int>& v) {
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i - 1] >= v[i])
            return false;
    }
    return true;
}

// Removes a key, then prints the inorder traversal and checks it is sorted
void removeAndShow(BinarySearchTree& tree, int key, const string& caseName) {
    cout << "\n--- Removing " << key << " (" << caseName << ") ---\n";
    tree.remove(key);
    vector<int> in = tree.inorder();
    printVector("Inorder:   ", in);
    cout << "Sorted?    " << (isSorted(in) ? "yes" : "NO (bug!)") << "\n";
    cout << "Height:    " << tree.height() << "\n";
}

// ---------- main ----------
int main() {
    BinarySearchTree tree;

    // Insert 12 distinct integers
    //
    /*
                 50
               /    \
             30      70
            /  \    /  \
          20   40  60   80
         /    /  \   \    \
       10   35   45   65   90
    */
    vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 35, 45, 65, 90};
    for (int v : values)
        tree.insert(v);

    cout << "=== Initial tree ===\n";
    printVector("Inorder:   ", tree.inorder());
    printVector("Preorder:  ", tree.preorder());
    printVector("Postorder: ", tree.postorder());
    cout << "Height:    " << tree.height() << "\n";

    // Search demo
    cout << "\n=== Search ===\n";
    cout << "search(65) -> " << (tree.search(65) ? "found" : "not found") << "\n";
    cout << "search(99) -> " << (tree.search(99) ? "found" : "not found") << "\n";

    // Removal demo (all three cases)
    cout << "\n=== Removal ===\n";
    removeAndShow(tree, 10, "leaf node");
    removeAndShow(tree, 60, "one child: only has right child 65");
    removeAndShow(tree, 30, "two children: replaced by successor 35");

    cout << "\n=== Final tree ===\n";
    printVector("Inorder:   ", tree.inorder());
    printVector("Preorder:  ", tree.preorder());
    printVector("Postorder: ", tree.postorder());

    return 0;
}

/*
 ---------------- Complexity ----------------
 h = height of the tree, n = number of nodes

 Operation | Average case (balanced-ish) | Worst case (skewed / linked list)
 ----------|-----------------------------|----------------------------------
 Search    | O(log n)                    | O(n)
 Insert    | O(log n)                    | O(n)
 Remove    | O(log n)                    | O(n)

 Each operation walks down one path from the root, so its cost is O(h).
 - Average: with random insertions the tree stays fairly balanced, so
   h is about log2(n).
 - Worst: inserting already-sorted keys (1, 2, 3, ...) makes every node
   have only one child, so the tree is a linked list and h = n.
 Remove also needs findMin for the two-children case, but that is still
 just one more walk down, so it stays O(h).
 Traversals visit every node once: O(n). Height is also O(n).
*/
