/* 2_4. Обход дерева в порядке level-order */
#include <iostream>
#include <queue>
#include <assert.h>
#include <sstream>
#include <string>
#include <stack>

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
class BinaryTree
{
    struct Node
    {
        Node *left, *right;
        int key;
        Node(const int k) : left(nullptr), right(nullptr), key(k) {}
    };

public:
    BinaryTree(Comparator c = Comparator()) : root(nullptr), comp(c) {}
    ~BinaryTree()
    {
        if (root == nullptr)
            return;

        std::stack<Node *> s1;
        std::stack<Node *> s2;

        s1.push(root);
        while (!s1.empty())
        {
            Node *elem = s1.top();
            s1.pop();
            s2.push(elem);
            if (elem->left != nullptr)
                s1.push(elem->left);
            if (elem->right != nullptr)
                s1.push(elem->right);
        }

        while (!s2.empty())
        {
            Node *elem = s2.top();
            s2.pop();
            delete elem;
        }
    }

    void insert(const int key)
    {
        if (root == nullptr)
        {
            root = new Node(key);
            return;
        }

        int res = 0;
        Node *cur = root, *parent = root;
        while (cur)
        {
            parent = cur;
            res = comp(key, cur->key);
            if (res < 0)
                cur = cur->left;
            else
                cur = cur->right;
        }

        cur = new Node(key);
        if (res < 0)
            parent->left = cur;
        else
            parent->right = cur;
    }

    void remove(const int key)
    {
        if (root == nullptr)
            return;

        int res = comp(key, root->key);
        if (res == 0)
        {
            deleteNode(root);
            return;
        }

        Node *parent = root;
        Node *cur = res < 0 ? parent->left : parent->right;

        while (cur)
        {
            int res = comp(key, cur->key);
            if (res == 0)
            {
                deleteNode(comp(key, parent->left->key) == 0 ? parent->left : parent->right);
                return;
            }
            else
            {
                parent = cur;
                cur = res < 0 ? cur->left : cur->right;
            }
        }
    }

    void traversBFS(std::ostream &out)
    {
        if (root == nullptr)
            return;

        std::queue<Node *> q;
        q.push(root);
        while (!q.empty())
        {
            Node *node = q.front();
            out << node->key << ' ';
            q.pop();
            if (node->left != nullptr)
                q.push(node->left);
            if (node->right != nullptr)
                q.push(node->right);
        }
    }

private:
    void deleteNode(Node *&node)
    {
        if (node->left == nullptr) //нет левого поддерева
        {
            Node *right = node->right;
            delete node;
            node = right;
        }
        else if (node->right == nullptr) //нет правого поддерева
        {
            Node *left = node->left;
            delete node;
            node = left;
        }
        else //есть оба поддерева
        {
            Node *minParent = node;
            Node *min = node->right;
            while (min->left != nullptr)
            {
                minParent = min;
                min = min->left;
            }
            node->key = min->key;
            (minParent->left == min ? minParent->left : minParent->right) = min->right;
            delete min;
        }
    }

    Node *root;
    Comparator comp;
};

void processRequest(std::istream &in, std::ostream &out)
{
    size_t n = 0;
    in >> n;

    BinaryTree<> tree;
    for (size_t i = 0; i < n; ++i)
    {
        int num;
        in >> num;
        tree.insert(num);
    }

    tree.traversBFS(out);
}

void test()
{
    {
        std::stringstream in;
        in << "3\n2 1 3";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "2 1 3 ");
    }
    {
        std::stringstream in;
        in << "3\n1 2 3";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "1 2 3 ");
    }
    {
        std::stringstream in;
        in << "3\n3 1 2";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "3 1 2 ");
    }
    {
        std::stringstream in;
        in << "4\n3 1 4 2";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "3 1 4 2 ");
    }
    {
        std::stringstream in;
        in << "15\n10 5 15 0 7 12 17 -5 3 6 8 11 13 16 20";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "10 5 15 0 7 12 17 -5 3 6 8 11 13 16 20 ");
    }
    std::cout << "tests passed\n";
}

int main()
{
    //test();
    processRequest(std::cin, std::cout);

    return 0;
}