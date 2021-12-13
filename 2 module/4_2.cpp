/* 4_2. Порядковые статистики */

#include <iostream>
#include <assert.h>
#include <sstream>

struct DefaultComparator
{
    int operator()(const int lhs, const int rhs) const
    {
        if (lhs < rhs)
            return -1;
        else if (lhs > rhs)
            return 1;
        return 0;
    }
};

template <class Comparator = DefaultComparator>
class AVLTree
{
    struct Node
    {
        Node *left, *right;
        int key, num_nodes;
        int8_t height;

        Node(const int k) : left(nullptr),
                            right(nullptr),
                            key(k),
                            num_nodes(1),
                            height(1)
        {
        }
        ~Node()
        {
            delete left;
            delete right;
        }
    };

public:
    AVLTree(Comparator c = Comparator()) : root(nullptr), comp(c) {}
    ~AVLTree()
    {
        delete root;
    }

    bool has(const int key) const
    {
        return hasInternal(key, root);
    }

    void insert(const int key)
    {
        root = insertInternal(key, root);
    }

    void erase(const int key)
    {
        root = eraseInternal(key, root);
    }

    int kthElement(const int k)
    {
        return kthElementInternal(k, root);
    }

private:
    int kthElementInternal(const int k, Node *node)
    {
        int del = k - numberOfNodes(node->left);
        if (del == 0)
            return node->key;
        else if (del < 0)
            return kthElementInternal(k, node->left);
        else
            return kthElementInternal(del - 1, node->right);
    }

    bool hasInternal(const int key, Node *node) const
    {
        if (node == nullptr)
            return false;

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1)
            return hasInternal(key, node->left);
        else if (cmp_res == 1)
            return hasInternal(key, node->right);
        else
            return true;
    }

    Node *insertInternal(const int key, Node *node)
    {
        if (node == nullptr)
            return new Node(key);

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1)
            node->left = insertInternal(key, node->left);
        else if (cmp_res == 1)
            node->right = insertInternal(key, node->right);

        return balance(node);
    }

    int8_t height(Node *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int8_t bfactor(Node *node)
    {
        return height(node->right) - height(node->left);
    }

    int numberOfNodes(const Node *node) const
    {
        if (node == nullptr)
            return 0;
        return node->num_nodes;
    }

    void fix_node(Node *node)
    {
        if (node == nullptr)
            return;
        node->height = 1 + std::max(height(node->left),
                                    height(node->right));
        node->num_nodes = 1 + numberOfNodes(node->left) + numberOfNodes(node->right);
    }

    Node *rotate_left(Node *node)
    {
        Node *top = node->right;
        node->right = top->left;
        top->left = node;

        fix_node(node);
        fix_node(top);
        return top;
    }

    Node *rotate_right(Node *node)
    {
        Node *top = node->left;
        node->left = top->right;
        top->right = node;

        fix_node(node);
        fix_node(top);
        return top;
    }

    Node *balance(Node *node)
    {
        fix_node(node);
        int8_t bf = bfactor(node);
        if (bf == 2)
        {
            if (bfactor(node->right) < 0)
                node->right = rotate_right(node->right);
            return rotate_left(node);
        }
        else if (bf == -2)
        {
            if (bfactor(node->left) > 0)
                node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        return node;
    }

    Node *find_min(Node *node)
    {
        if (node->left == nullptr)
            return node;
        return find_min(node->left);
    }

    Node *remove_min(Node *node)
    {
        if (node->left == nullptr)
            return node->right;
        node->left = remove_min(node->left);
        return balance(node);
    }

    Node *find_remove_min(Node *node, Node *&min_node)
    {
        if (node->left == nullptr)
        {
            min_node = node;
            return node->right;
        }
        node->left = find_remove_min(node->left, min_node);
        return balance(node);
    }

    Node *eraseInternal(const int key, Node *node)
    {
        if (node == nullptr)
            return nullptr;

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1)
            node->left = eraseInternal(key, node->left);
        else if (cmp_res == 1)
            node->right = eraseInternal(key, node->right);
        else
        {
            Node *left = node->left;
            Node *right = node->right;

            node->right = nullptr;
            node->left = nullptr;

            delete node;

            if (right == nullptr)
                return left;

            Node *min_node;
            right = find_remove_min(right, min_node);
            min_node->left = left;
            min_node->right = right;

            return balance(min_node);
        }
        return balance(node);
    }

    Node *root;
    Comparator comp;
};

void processRequest(std::istream &in, std::ostream &out)
{
    size_t N = 0;
    in >> N;

    AVLTree<> tree;
    for (size_t i = 0; i < N; ++i)
    {
        int num = 0, k = 0;
        in >> num >> k;
        if (num >= 0)
            tree.insert(num);
        else
            tree.erase(-num);
        out << tree.kthElement(k) << '\n';
    }
}

void testAVLTree()
{
    {
        AVLTree<> avltree;

        assert(!avltree.has(1));

        avltree.insert(1);
        assert(avltree.has(1));

        avltree.erase(1);
        assert(!avltree.has(1));
    }

    {
        AVLTree<> avltree;
        for (int i = 0; i < 100; ++i)
        {
            assert(!avltree.has(i));
            avltree.insert(i);
        }
        for (int i = 0; i < 100; ++i)
        {
            assert(avltree.has(i));
            avltree.erase(i);
            assert(!avltree.has(i));
        }
    }

    std::cout << "AVLTree tests passed\n";
}

void testKthElement()
{
    {
        AVLTree<> tree;
        for (int i = 0; i < 100; ++i)
            tree.insert(i);

        for (int i = 0; i < 100; ++i)
            assert(i == tree.kthElement(i));
    }

    std::cout << "kthElement tests passed\n";
}

void testRequest()
{
    {
        std::stringstream in;
        in << "3\n1 0\n2 0\n-1 0";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "1\n1\n2\n");
    }
    {
        std::stringstream in;
        in << "5\n40 0\n10 1\n4 1\n-10 0\n50 2";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "40\n40\n10\n4\n50\n");
    }
    std::cout << "Request tests passed\n";
}

int main()
{
    //testAVLTree();
    //testKthElement();
    //testRequest();
    processRequest(std::cin, std::cout);
    return 0;
}