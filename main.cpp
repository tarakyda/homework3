#include <iostream>
#include <initializer_list>
#include <fstream>

class BSTree {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        Node* parent; // опционально

        Node(int value, Node* parent = nullptr) : data(value), left(nullptr), right(nullptr), parent(parent) {}
    };

    Node* root;

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node* add_element(Node* node, int value, Node* parent) {
        if (!node) {
            return new Node(value, parent);
        }
        if (value < node->data) {
            node->left = add_element(node->left, value, node);
        } else if (value > node->data) {
            node->right = add_element(node->right, value, node);
        }
        return node;
    }

    Node* find_element(Node* node, int value) {
        if (!node || node->data == value) {
            return node;
        }
        if (value < node->data) {
            return find_element(node->left, value);
        } else {
            return find_element(node->right, value);
        }
    }

    Node* delete_element(Node* node, int value) {
        if (!node) return node;
        if (value < node->data) {
            node->left = delete_element(node->left, value);
        } else if (value > node->data) {
            node->right = delete_element(node->right, value);
        } else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = min_value_node(node->right);
            node->data = temp->data;
            node->right = delete_element(node->right, temp->data);
        }
        return node;
    }

    Node* min_value_node(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void in_order_print(Node* node) const {
        if (node) {
            in_order_print(node->left);
            std::cout << node->data << " ";
            in_order_print(node->right);
        }
    }

    void save_to_file(Node* node, std::ofstream& ofs) const {
        if (node) {
            ofs << node->data << " ";
            save_to_file(node->left, ofs);
            save_to_file(node->right, ofs);
        }
    }

    Node* load_from_file(std::ifstream& ifs, Node* parent) {
        int value;
        if (ifs >> value) {
            Node* node = new Node(value, parent);
            node->left = load_from_file(ifs, node);
            node->right = load_from_file(ifs, node);
            return node;
        }
        return nullptr;
    }

public:
    BSTree() : root(nullptr) {}

    BSTree(std::initializer_list<int> list) : root(nullptr) {
        for (int value : list) {
            add_element(value);
        }
    }

    ~BSTree() {
        clear(root);
    }

    bool add_element(int value) {
        if (!find_element(root, value)) {
            root = add_element(root, value, nullptr);
            return true;
        }
        return false;
    }

    bool delete_element(int value) {
        if (find_element(root, value)) {
            root = delete_element(root, value);
            return true;
        }
        return false;
    }

    bool find_element(int value) {
        return find_element(root, value) != nullptr;
    }

    void print() const {
        in_order_print(root);
        std::cout << std::endl;
    }

    bool save_to_file(const std::string& path) {
        std::ofstream ofs(path);
        if (!ofs) return false;
        save_to_file(root, ofs);
        ofs.close();
        return true;
    }

    bool load_from_file(const std::string& path) {
        std::ifstream ifs(path);
        if (!ifs) return false;
        clear(root);
        root = load_from_file(ifs, nullptr);
        ifs.close();
        return true;
    }
};

int main() {
    BSTree tree = {5, 3, 7, 2, 4, 6, 8};
    tree.print();

    tree.add_element(1);
    tree.print();

    tree.delete_element(3);
    tree.print();

    tree.save_to_file("tree.txt");
    BSTree new_tree;
    new_tree.load_from_file("tree.txt");
    new_tree.print();

    return 0;
}
