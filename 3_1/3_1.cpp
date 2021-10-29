//Реализовать очередь с динамическим зацикленным буфером.
//Обрабатывать команды push back и pop front.
//
//Если дана команда pop front, то число b - ожидаемое значение.
//Если команда pop front вызвана для пустой структуры данных, то ожидается “ - 1”.
//
//Требуется напечатать YES - если все ожидаемые значения совпали.
//Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

#include <iostream>
#include <sstream>
#include <assert.h>
#define MIN_QUEUE_CAPACITY 8
#define RESIZE_FACTOR 2



class Queue
{
public:
    Queue()
    {
        capacity = MIN_QUEUE_CAPACITY;
        size = 0;
        data = new int[capacity];
        front = -1;
        back = -1;
    }

    ~Queue()
    {
        delete[] data;
    }

    void push(int var)
    {
        if (full())
        {
            int* new_data = new int[capacity * RESIZE_FACTOR];
            for (int i = 0; i < size; ++i)
            {
                new_data[front + i] = data[(front + i) % capacity];
            }
            back = front + size - 1;
            capacity *= RESIZE_FACTOR;

            delete[] data;
            data = new_data;
        }

        if (empty())
            front = 0;
        
        back = (back + 1) % capacity;
        data[back] = var;
        size++;
    }

    int pop()
    {
        if (empty())
            return -1;

        int tmp = data[front];
        if (front == back) // в структуре только один элемент, теперь она пустая
        {
            front = -1;
            back = -1;
        }
        else
            front = (front + 1) % capacity;

        size--;
        return tmp;
    }

    void print(std::ostream& output)
    {
        if (!empty())
        {
            output << data[front];
            for (int i = 1; i < size; ++i)
                output << ' ' << data[(front + i) % capacity];
        }
    }

private:
    bool empty()
    {
        return (front == -1 && back == -1);
    }

    bool full()
    {
        return (size == capacity);
    }


    int* data;
    int size, capacity, front, back;
};

void request(std::istream& in, std::ostream& out)
{
    int n = 0;
    in >> n;

    Queue q;
    bool flag = true;

    for (int i = 0; i < n; ++i)
    {
        int a = 0, b = 0;
        in >> a >> b;

        if (a == 3)
            q.push(b);

        if (a == 2)
            if (q.pop() != b)
                flag = false;
    }

    out << (flag ? "YES" : "NO");
}

void test_Queue()
{
    {
        Queue q;

        std::stringstream s;
        q.print(s);

        assert(s.str() == "");
        assert(q.pop() == -1);
    }

    {
        Queue q;
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
        
        std::stringstream s;
        q.print(s);

        assert(s.str() == "1 2 3 4 5");
    }

    {
        Queue q;
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
        q.pop();
        q.pop();

        std::stringstream s;
        q.print(s);

        assert(s.str() == "3 4 5");
    }

    {
        Queue q;

        for (int i = 0; i < 100; ++i)
            q.push(i);

        for (int i = 0; i < 100; ++i)
            assert(q.pop() == i);
    }

    std::cout << "ALL QUEUE TESTS PASSED" << std::endl;
}

void test_request()
{
    {
        std::stringstream in;
        in << "3\n";
        in << "3 44\n";
        in << "3 50\n";
        in << "2 44\n";

        std::stringstream out;
        request(in, out);
        
        assert(out.str() == "YES");
    }

    {
        std::stringstream in;
        in << "2\n";
        in << "2 -1\n";
        in << "3 10\n";

        std::stringstream out;
        request(in, out);

        assert(out.str() == "YES");
    }

    {
        std::stringstream in;
        in << "2\n";
        in << "3 44\n";
        in << "2 66\n";

        std::stringstream out;
        request(in, out);

        assert(out.str() == "NO");
    }

    {
        std::stringstream in;
        in << "10\n";

        in << "3 1\n";
        in << "3 2\n";
        in << "3 3\n";
        in << "3 4\n";
        in << "3 5\n";

        in << "2 1\n";
        in << "2 2\n";
        in << "2 3\n";
        in << "2 4\n";
        in << "2 5\n";

        std::stringstream out;
        request(in, out);

        assert(out.str() == "YES");
    }

    std::cout << "ALL REQUEST TESTS PASSED" << std::endl;
}


int main()
{
    test_Queue();
    test_request();
    //request(std::cin, std::cout);


    return 0;
}