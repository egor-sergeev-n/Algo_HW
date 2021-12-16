/* 3. B-дерево */

#include <iostream>
#include <vector>
#include <assert.h>
#include <queue>
#include <sstream>

template <class Key>
struct DefaultComparator
{
    int operator()(const Key &lhs, const Key &rhs) const
    {
        return lhs < rhs;
    }
};

template <class Key, class Comparator = DefaultComparator<Key>>
class BTree
{
public:
    struct Node
    {
        bool leaf;
        std::vector<Key> keys;
        std::vector<Node *> children;

        Node(bool leaf) : leaf(leaf) {}
        ~Node()
        {
            for (Node *child : children)
                delete child;
        }
    };

    BTree(int min_degree) : root(nullptr), t(min_degree)
    {
    }
    ~BTree()
    {
        delete root;
    }

    std::pair<Node *, int> search(const Key &key) const
    {
        return searchInternal(key, root);
    }

    void insert(const Key &key)
    {
        if (root == nullptr)
            root = new Node(true);

        if (isFullNode(root))
        {
            Node *new_root = new Node(false);
            new_root->children.push_back(root);
            root = new_root;
            splitChild(root, 0);
        }

        insertNonFull(key, root);
    }

    void traversBFS(std::ostream &out)
    {
        if (root == nullptr)
            return;

        std::queue<Node *> q1;
        q1.push(root);
        while (!q1.empty())
        {
            std::queue<Node *> q2;
            while (!q1.empty())
            {
                Node *node = q1.front();
                q1.pop();
                for (int i = 0; i < node->keys.size(); ++i)
                    out << node->keys[i] << ' ';

                for (Node *child : node->children)
                    if (child != nullptr)
                        q2.push(child);
            }

            while (!q2.empty())
            {
                q1.push(q2.front());
                q2.pop();
            }

            out << '\n';
        }
    }

private:
    void insertNonFull(const Key &key, Node *node)
    {
        int pos = node->keys.size() - 1;
        if (node->leaf)
        {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && key < node->keys[pos])
            {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else
        {
            while (pos >= 0 && key < node->keys[pos])
                pos--;

            if (isFullNode(node->children[pos + 1]))
            {
                splitChild(node, pos + 1);
                if (key > node->keys[pos + 1])
                    pos++;
            }

            insertNonFull(key, node->children[pos + 1]);
        }
    }

    void splitChild(Node *node, int idx)
    {
        Node *Y = node->children[idx];
        Node *Z = new Node(Y->leaf);

        Z->keys.resize(t - 1);
        for (int j = 0; j < t - 1; ++j)
            Z->keys[j] = Y->keys[j + t];

        Z->children.resize(t);
        if (!Y->leaf)
            for (int j = 0; j < t; ++j)
                Z->children[j] = Y->children[j + t];

        Key &median_key = Y->keys[t - 1];
        Y->keys.resize(t - 1);
        Y->children.resize(t);
        node->children.resize(node->children.size() + 1);
        for (int j = node->children.size() - 2; j > idx; --j)
            node->children[j + 1] = node->children[j];
        node->children[idx + 1] = Z;

        node->keys.resize(node->keys.size() + 1);
        for (int j = node->keys.size() - 2; j >= idx; --j)
            node->keys[j + 1] = node->keys[j];
        node->keys[idx] = median_key;
    }

    bool isFullNode(Node *node) const
    {
        return node->keys.size() == 2 * t - 1;
    }

    std::pair<Node *, int> searchInternal(const Key &key, Node *node) const
    {
        int pos = 0;
        while (pos < node->keys.size() && key > node->keys[pos])
            pos++;

        if (pos < node->keys.size() && key == node->keys[pos])
            return {node, pos};

        if (node->leaf)
            return {nullptr, 0};

        return searchInternal(key, node->children[pos]);
    }

    Node *root;
    int t;
};

void processRequest(std::istream &in, std::ostream &out)
{
    size_t t = 0;
    in >> t;

    BTree<int> tree(t);
    int num;
    while (in >> num)
        tree.insert(num);

    tree.traversBFS(out);
}

void testBTree()
{
    {
        BTree<int> tree(5);
        for (int i = 0; i < 100; ++i)
            tree.insert(i);

        for (int i = 0; i < 100; ++i)
            assert(tree.search(i).first != nullptr);
    }

    {
        std::stringstream in, out;
        in << "2\n0 1 2 3 4 5 6 7 8 9";
        processRequest(in, out);
        assert(out.str() == "3 \n1 5 7 \n0 2 4 6 8 9 \n");
    }

    {
        std::stringstream in, out;
        in << "4\n0 1 2 3 4 5 6 7 8 9";
        processRequest(in, out);
        assert(out.str() == "3 \n0 1 2 4 5 6 7 8 9 \n");
    }

    std::cout << "Tests passed\n";
}

int main()
{
    testBTree();
    //processRequest(std::cin, std::cout);
    return 0;
}