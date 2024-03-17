#pragma once

#include <iostream>  // For debugging
#include <sstream>   // For as_string

using namespace std;

template <typename T>
class prqueue {
   private:
    struct NODE {
        int priority;
        T value;
        NODE* parent;
        NODE* left;
        NODE* right;
        NODE* link;  // Link to duplicates -- Part 2 only
    };

    NODE* root;
    size_t sz;

    // Utility pointers for begin and next.
    NODE* curr;
    NODE* temp;  // Optional

    // TODO_STUDENT: add private helper function definitions here
    void _recursiveHelper(const NODE* node, ostream& output) const {
        if (node != nullptr) {
            _recursiveHelper(node->left, output);

            // Append the priority and value to the output stream
            output << node->priority << " value: " << node->value << endl;

            // Check and append duplicate values
            NODE* current = node->link;
            while (current != nullptr) {
                output << node->priority << " value: " << current->value << endl;
                current = current->link;
            }

            _recursiveHelper(node->right, output);
        }
    }

    void removeNode(NODE* node) {
        if (node->parent != nullptr) {
            if (node->parent->left == node) {
                node->parent->left = node->right;
            }
            else {
                node->parent->right = node->right;
            }
        }
        else {
            root = node->right;
        }

        if (node->right != nullptr) {
            node->right->parent = node->parent;
        }

        delete node;
        sz--;
    }

    void _clearRecursive(NODE* node) {
        if (node != nullptr) {
            _clearRecursive(node->left);
            _clearRecursive(node->right);
            
            // Check if the node has duplicates
            NODE* current = node->link;
            while (current != nullptr) {
                NODE* next = current->link;
                delete current;
                current = next;
            }

            delete node;
        }
    }

   public:
    /// Creates an empty `prqueue`.
    /// Runs in O(1).
    prqueue() {
        root = nullptr;
        sz = 0;
        curr = nullptr;
        temp = nullptr;
    }

    /// Copy constructor.
    ///
    /// Copies the value-priority pairs from the provided `prqueue`.
    /// The internal tree structure must be copied exactly.
    ///
    /// Runs in O(N), where N is the number of values in `other`.
    prqueue(const prqueue& other) {
        // TODO_STUDENT
    }

    /// Assignment operator; `operator=`.
    ///
    /// Clears `this` tree, and copies the value-priority pairs from the
    /// provided `prqueue`. The internal tree structure must be copied exactly.
    ///
    /// Runs in O(N + O), where N is the number of values in `this`, and O is
    /// the number of values in `other`.
    prqueue& operator=(const prqueue& other) {/*
        if (this != &other) { // Check for self-assignment
            // Clear the current tree
            clear();

            // Copy values and internal tree structure from other
            copyTree(other.root, root);
        }

        return *this;*/
    }

    /// Empties the `prqueue`, freeing all memory it controls.
    ///
    /// Runs in O(N), where N is the number of values.
    void clear() {
        _clearRecursive(root); // Assuming 'root' is the root of your priority queue
        root = nullptr; // Reset the root to nullptr after clearing
        sz = 0;
    }

    /// Destructor, cleans up all memory associated with `prqueue`.
    ///
    /// Runs in O(N), where N is the number of values.
    ~prqueue() {
        clear();
    }

    /// Adds `value` to the `prqueue` with the given `priority`.
    ///
    /// Uses the priority to determine the location in the underlying tree.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    void enqueue(T value, int priority) {
        NODE* newNode = new NODE;
        newNode->value = value;
        newNode->priority = priority;
        newNode->parent = nullptr;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->link = nullptr;

        // If the tree is empty, the new node becomes the root
        if (root == nullptr) {
            root = newNode;
            sz++;
            return;
        }

        // Otherwise, find where to insert the new node
        NODE* current = root;
        NODE* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            // Check for duplicate priorities
            if (priority == current->priority) {
                // Attach the new node to the linked list of duplicates
                while (current->link != nullptr) {
                    current = current->link;
                }
                current->link = newNode;
                newNode->parent = current;
                sz++;
                return;
            }

            // Assuming lower priority values are higher in the queue
            if (priority < current->priority) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        // Attach the new node
        if (priority < parent->priority) {
            parent->left = newNode;
        }
        else {
            parent->right = newNode;
        }

        newNode->parent = parent;
        sz++;
    }

    /// Returns the value with the smallest priority in the `prqueue`, but does
    /// not modify the `prqueue`.
    ///
    /// If the `prqueue` is empty, returns the default value for `T`.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    T peek() const {
        if (root == nullptr) {
            return T{};
        }

        // Find the node with the lowest priority
        NODE* current = root;
        while (current->left != nullptr) {
            current = current->left;
        }

        return current->value;
    }

    /// Returns the value with the smallest priority in the `prqueue` and
    /// removes it from the `prqueue`.
    ///
    /// If the `prqueue` is empty, returns the default value for `T`.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    T dequeue() {
        if (root == nullptr) {
            return T();
        }

        // Find the leftmost node
        NODE* current = root;
        while (current->left != nullptr) {
            current = current->left;
        }

        T result = current->value;

        // Remove the leftmost node
        removeNode(current);

        return result;
    }

    /// Returns the number of elements in the `prqueue`.
    ///
    /// Runs in O(1).
    size_t size() const {
        return sz;
    }

    /// Resets internal state for an iterative inorder traversal.
    ///
    /// See `next` for usage details.
    ///
    /// O(H), where H is the maximum height of the tree.
    void begin() {
        curr = root;

        // Move to the leftmost node
        while (curr != nullptr && curr->left != nullptr) {
            curr = curr->left;
        }
    }

    /// Uses the internal state to return the next in-order value and priority
    /// by reference, and advances the internal state. Returns true if the
    /// reference parameters were set, and false otherwise.
    ///
    /// Example usage:
    ///
    /// ```c++
    /// pq.begin();
    /// T value;
    /// int priority;
    /// while (pq.next(value, priority)) {
    ///   cout << priority << " value: " << value << endl;
    /// }
    /// ```
    ///
    /// Runs in worst-case O(H + M) or O(H), depending on implementation, where
    /// H is the height of the tree, and M is the number of duplicate
    /// priorities.
    bool next(T& value, int& priority) {
        if (curr != nullptr) {
            value = curr->value;
            priority = curr->priority;

            if (curr->right != nullptr) {
                // If there's a right child, move to the leftmost node of the right subtree
                curr = curr->right;
                while (curr->left != nullptr) {
                    curr = curr->left;
                }
            }
            else {
                // If no right child, move up to the parent until reaching a node where left child
                while (curr->parent != nullptr && curr->parent->right == curr) {
                    curr = curr->parent;
                }
                curr = curr->parent;
            }

            return true;
        }

        return false;
    }

    /// Converts the `prqueue` to a string representation, with the values
    /// in-order by priority.
    ///
    /// Example:
    ///
    /// ```c++
    /// prqueue<string> names;
    /// names.enqueue("Gwen", 3);
    /// names.enqueue("Jen", 2);
    /// names.enqueue("Ben", 1);
    /// names.enqueue("Sven", 2);
    /// ```
    ///
    /// Calling `names.as_string()` would return the following multi-line
    /// string:
    ///
    /// ```text
    /// 1 value: Ben
    /// 2 value: Jen
    /// 2 value: Sven
    /// 3 value: Gwen
    /// ```
    ///
    /// Runs in O(N), where N is the number of values.
    string as_string() const {
        ostringstream result;
        _recursiveHelper(root, result);
        return result.str();
    }

    /// Checks if the contents of `this` and `other` are equivalent.
    ///
    /// Two `prqueues` are equivalent if they have the same priorities and
    /// values, as well as the same internal tree structure.
    ///
    /// These two `prqueue`s would be considered equivalent, because
    /// they have the same internal tree structure:
    ///
    /// ```c++
    /// prqueue<string> a;
    /// a.enqueue("2", 2);
    /// a.enqueue("1", 1);
    /// a.enqueue("3", 3);
    /// ```
    ///
    /// and
    ///
    /// ```c++
    /// prqueue<string> b;
    /// a.enqueue("2", 2);
    /// a.enqueue("3", 3);
    /// a.enqueue("1", 1);
    /// ```
    ///
    /// While this `prqueue`, despite containing the same priority-value pairs,
    /// would not be considered equivalent, because the internal tree structure
    /// is different.
    ///
    /// ```c++
    /// prqueue<string> c;
    /// a.enqueue("1", 1);
    /// a.enqueue("2", 2);
    /// a.enqueue("3", 3);
    /// ```
    ///
    /// Runs in O(N) time, where N is the maximum number of nodes in
    /// either `prqueue`.
    ///
    bool operator==(const prqueue& other) const {
        
    }

    /// Returns a pointer to the root node of the BST.
    ///
    /// Used for testing the internal structure of the BST. Do not edit or
    /// change.
    ///
    /// Runs in O(1).
    void* getRoot() {
        return root;
    }
};
