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
        if (node == nullptr) {
            return;
        }

        NODE* parent = node->parent;

        // Node has no children
        if (node->left == nullptr && node->right == nullptr) {
            // If the node is not the root, adjust its parent's pointer
            if (parent != nullptr) {
                if (parent->left == node) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }
            }
            else {
                // If the node is the root, update the root pointer
                root = nullptr;
            }
            delete node;
        }
        // Node has one child
        else {
            NODE* child = (node->left != nullptr) ? node->left : node->right;
            // If the node is not the root, adjust its parent's pointer
            if (parent != nullptr) {
                if (parent->left == node) {
                    parent->left = child;
                }
                else {
                    parent->right = child;
                }
            }
            else {
                root = child;
            }
            child->parent = parent;
            delete node;
        }
        sz--;
    }

    void _clear(NODE* node) {
        if (node == nullptr) {
            return;
        }
        // Recursively clear left and right subtrees
        _clear(node->left);
        _clear(node->right);
        
        // Clear linked list of duplicates
        while (node->link != nullptr) {
            NODE* delVal = node->link;
            node->link = node->link->link;
            delete delVal;
        }

        delete node;
    }
    
    /// Recursive helper function to copy nodes.
    void copyTree(NODE*& current, const NODE* otherNode) {
        if (otherNode == nullptr) {
            return;
        }
        // Copy the current node's value and priority
        enqueue(otherNode->value, otherNode->priority);

        copyTree(current->left, otherNode->left);
        copyTree(current->right, otherNode->right);

        // If there's a linked list of nodes with the same priority, copy it
        while (otherNode->link != nullptr) {
            enqueue(otherNode->link->value, otherNode->link->priority);
            otherNode = otherNode->link;
        }
    }

    // Recursive helper function to check if two trees are equivalent.
    bool isEqual(NODE* node1, NODE* node2) const {
        if (!node1 && !node2) return true; // Both trees are empty
        if (!node1 || !node2) return false; // One tree is empty, the other is not

        // Check if priorities and values are equal
        if (node1->priority != node2->priority || node1->value != node2->value) return false;

        // Recursively check left and right subtrees
        return isEqual(node1->left, node2->left) && isEqual(node1->right, node2->right);
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
        root = nullptr;
        copyTree(root, other.root);
        sz = other.sz;
    }

    /// Assignment operator; `operator=`.
    ///
    /// Clears `this` tree, and copies the value-priority pairs from the
    /// provided `prqueue`. The internal tree structure must be copied exactly.
    ///
    /// Runs in O(N + O), where N is the number of values in `this`, and O is
    /// the number of values in `other`.
    prqueue& operator=(const prqueue& other) {
        if (this == &other) {
            return *this; // Avoid self-assignment
        }
        clear();

        // Call the recursive function to copy the tree structure and values
        copyTree(root, other.root);
        sz = other.sz;
        return *this;
    }

    /// Empties the `prqueue`, freeing all memory it controls.
    ///
    /// Runs in O(N), where N is the number of values.
    void clear() {
        _clear(root);
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
        sz++;

        // If the tree is empty, the new node becomes the root
        if (root == nullptr) {
            root = newNode;
            return;
        }

        // Otherwise, find where to insert the new node
        NODE* current = root;
        NODE* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (priority == current->priority) {
                while (current->link != nullptr) {
                    current = current->link;
                }
                current->link = newNode;
                newNode->parent = parent;
                return;
            }
            else if (priority < current->priority) {
                if (current->left == nullptr) {
                    current->left = newNode;
                    newNode->parent = parent;
                    return;
                }
                current = current->left;
            }
            else {
                if (current->right == nullptr) {
                    current->right = newNode;
                    newNode->parent = parent;
                    return;
                }
                current = current->right;
            }
        }
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

        // If has dupes
        if (current->link != nullptr) {
            temp = current->link;
            current->value = current->link->value;
            current->link = current->link->link;
            delete temp;
        }
        else {
            removeNode(current);
        }
        
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
        if (curr == nullptr) {
            return false;
        }

        // Current values
        value = curr->value;
        priority = curr->priority;

        // Find largest priority node
        temp = root;
        while(temp->right != nullptr) {
            temp = temp->right;
        }
        // If current node is largest priority (last) node && current priority is less than last priority, we're done
        if (temp == curr && curr->priority < priority) {
            return false;
        }

        // If current node is largest priority (last) node && current priority is equal to last priority, we're on the last node
        if (temp == curr && curr->priority == priority) {
            // If current still has dupes, we have to address those
            if (curr->link != nullptr) {
                curr = curr->link;
                return true;
            }
            
            // If curr is a node in the linked list
            if (curr->parent != nullptr && curr->parent->priority == curr->priority) {
                return false;
            }
            
            curr = nullptr;
            return true;
        }

        // If has next dupe, traverse to next dupe
        if (curr->link != nullptr) {
            curr = curr->link;
            return true;
        }

        // If curr is a node in the linked list, reset to head node of linked list
        if (curr->parent != nullptr) {
            if (temp->priority == curr->priority && curr->link == nullptr) {
                curr = nullptr;
                return true;
            }
            if (curr->priority == curr->parent->priority) {
                curr = curr->parent;
            }
        }
        
        // If has a right child, move to it and then to the leftmost node of its right subtree
        if (curr->right != nullptr && curr->right->priority > priority) {
            curr = curr->right;
            while (curr->left != nullptr) {
                curr = curr->left;
            }
            return true;
        }
        
        // If has no valid children
        // If is left node of parent, move to parent
        if (curr->parent->left == curr) {
            curr = curr->parent;
            return true;
        }

        // Else is right node of parent with no valid children
        // Traverse upwards through parents until has valid right nodes or no parent
        while (curr->parent != nullptr) {
            // If there's a right child, travel to right and move to leftmost node of its right subtree
            if (curr->parent->right != nullptr && curr->parent->right->priority > priority) {
                curr = curr->parent;
                if (curr->priority < priority) {
                    if (curr->right->priority > priority) {
                        curr = curr->right;
                        while (curr->left != nullptr) {
                            curr = curr->left;
                        }
                    }
                }
                return true;
            }
            else if (curr->parent->left == curr && curr->parent->priority > priority) {
                curr = curr->parent;
                return true;
            }
            curr = curr->parent;
        }
        
        // We shouldn't reach here
        return true;    
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
        return isEqual(root, other.root);
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
