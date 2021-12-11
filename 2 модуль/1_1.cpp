/* 1. Хеш-таблица
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. */

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <sstream>

template <class T>
struct Hasher
{
    size_t operator()(const T &key) const
    {
        return key;
    }
};

template <>
struct Hasher<std::string>
{
    size_t operator()(const std::string &key) const
    {
        size_t hash = 0;
        for (char ch : key)
            hash = hash * 13 + ch;
        return hash;
    }
};

template <class T>
struct DefaultEqual
{
    bool operator()(const T &lhs, const T &rhs) const
    {
        return lhs == rhs;
    }
};

template <class Key, class Equal = DefaultEqual<Key>, class Hash = Hasher<Key>>
class HashSet
{
    static constexpr float MAX_LOAD_FACTOR = 0.75;
    enum State
    {
        EMPTY,
        VALID,
        DELETED
    };
    struct Item
    {
        Key key;
        State state;
        Item() : key(Key()), state(State::EMPTY) {}
    };

public:
    HashSet(Hash hash = Hash(), Equal equal = Equal()) : data(8),
                                                         hash(hash),
                                                         equal(equal),
                                                         items_count(0)
    {
    }

    ~HashSet() {}

    HashSet(const HashSet &) = delete;
    HashSet &operator=(const HashSet &) = delete;

    bool add(const Key &key)
    {
        if (items_count > (float)data.size() * MAX_LOAD_FACTOR)
            grow();
        return add_to_vector(key, data);
    }

    bool has(const Key &key) const
    {
        size_t h = hash(key);
        for (size_t i = 0; i < data.size(); ++i)
        {
            h = (h + i) % data.size();

            switch (data[h].state)
            {
            case State::EMPTY:
                return false;

            case State::VALID:
            {
                if (equal(data[h].key, key))
                    return true;
                break;
            }
            default:
                break;
            }
        }

        return false;
    }

    bool remove(const Key &key)
    {
        size_t h = hash(key);
        for (size_t i = 0; i < data.size(); ++i)
        {
            h = (h + i) % data.size();

            switch (data[h].state)
            {
            case State::EMPTY:
                return false;

            case State::VALID:
            {
                if (equal(data[h].key, key))
                {
                    data[h].state = State::DELETED;
                    items_count--;
                    if (items_count == 0)
                    {
                        data.resize(8);
                        std::fill(data.begin(), data.end(), Item());
                    }
                    return true;
                }
                break;
            }
            default:
                break;
            }
        }

        return false;
    }

private:
    void grow()
    {
        items_count = 0;
        std::vector<Item> new_data(2 * data.size());
        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i].state == State::VALID)
            {
                add_to_vector(data[i].key, new_data);
            }
        }

        data = std::move(new_data);
    }

    bool add_to_vector(const Key &key, std::vector<Item> &vec)
    {
        size_t h = hash(key);
        int deleted_idx = -1;
        for (size_t i = 0; i < vec.size(); ++i)
        {
            h = (h + i) % vec.size();

            switch (vec[h].state)
            {
            case State::DELETED:
            {
                if (deleted_idx == -1)
                    deleted_idx = h;
                break;
            }

            case State::EMPTY:
            {
                if (deleted_idx != -1)
                    h = deleted_idx;

                vec[h].key = key;
                vec[h].state = State::VALID;
                items_count++;
                return true;
            }

            case State::VALID:
            {
                if (equal(vec[h].key, key))
                    return false;
                break;
            }
            default:
                break;
            }
        }

        if (deleted_idx == -1)
            return false;

        vec[deleted_idx].key = key;
        vec[deleted_idx].state = State::VALID;
        items_count++;
        return true;
    }

    std::vector<Item> data;
    Hash hash;
    Equal equal;
    size_t items_count;
};

void processRequest(std::istream &in, std::ostream &out)
{
    HashSet<std::string> set;
    std::string key;
    char operation = '\0';
    while (in >> operation >> key)
    {
        bool res = false;
        switch (operation)
        {
        case '+':
            res = set.add(key);
            break;

        case '-':
            res = set.remove(key);
            break;

        case '?':
            res = set.has(key);
            break;

        default:
            break;
        }

        std::cout << (res ? "OK" : "FAIL") << '\n';
    }
}

void test_hashSet()
{
    {
        HashSet<int> set;
        for (int i = 0; i < 100; ++i)
        {
            assert(!set.has(i));
            assert(set.add(i));
        }

        for (int i = 0; i < 100; ++i)
        {
            assert(set.has(i));
            assert(set.remove(i));
        }
    }

    {
        HashSet<std::string> set;
        assert(!set.has("abc"));
        assert(set.add("abc"));
        assert(set.remove("abc"));
        assert(!set.has("abc"));

        assert(set.add("111"));
        assert(!set.add("111"));
        assert(set.remove("111"));
        assert(!set.remove("111"));
        assert(!set.has("111"));
    }

    std::cout << "HashSet tests passed\n";
}

int main()
{
    //test_hashSet();
    processRequest(std::cin, std::cout);

    return 0;
}