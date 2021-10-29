#include <iostream>
#define MIN_CAPACITY 3
#define INCREASE_FACTOR 2


template <typename T>
class Heap
{
public:
    Heap()
    {
        arr = new T[MIN_CAPACITY];
        capacity = MIN_CAPACITY;
        size = 0;
    }

    Heap(T* a, int s)
    {
        arr = a;
        capacity = (s > MIN_CAPACITY ? s : MIN_CAPACITY);
        size = s;
        build_heap();
    }

    Heap(T* a, int s, bool (*c)(const T&, const T&))
    {
        arr = a;
        size = s;
        capacity = (s > MIN_CAPACITY ? s : MIN_CAPACITY);
        comp = c;
        build_heap();
    }

    ~Heap()
    {
        delete[] arr;
    }

    void set_compare(bool (*c)(const T&, const T&))
    {
        comp = c;
    }

    void insert(T elem)
    {
        if (fool())
        {
            capacity *= INCREASE_FACTOR;

            T* new_arr = new T[capacity];
            for (int i = 0; i < size; ++i)
                new_arr[i] = arr[i];

            delete[] arr;
            arr = new_arr;
        }

        arr[size] = elem;
        sift_up(size);
        size++;
    }

    T extract()
    {
        if (empty())
            return NULL;

        T result = arr[0];
        arr[0] = arr[size - 1];

        if (!empty())
            sift_down(0);

        size--;
        return result;
    }

    void print()
    {
        if (!empty())
        {
            std::cout << arr[0] << std::endl;
            
            int left = 1;
            int right = 2;
            while (left < size)
            {
                for (int k = left; k <= right && k < size; ++k)
                    std::cout << arr[k] << ' ';
                left = left * 2 + 1;
                right = right * 2 + 2;
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }

private:
    void build_heap()
    {
        for(int i = (size - 1) / 2; i >= 0; --i)
            sift_down(i);
    }

    bool fool()
    {
        return size == capacity;
    }

    bool empty()
    {
        return size == 0;
    }

    void sift_down(int i)
    {
        while (true)
        {
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            int largest = i;
            if (left < size && comp(arr[left], arr[i]))
                largest = left;
            if (right < size && comp(arr[right], arr[largest]))
                largest = right;

            if (largest == i)
                return;

            std::swap(arr[i], arr[largest]);
            i = largest;
        }
    }

    void sift_up(int i)
    {
        while (i > 0)
        {
            int parent = (i - 1) / 2;
            if (comp(arr[parent], arr[i]))
                return;

            std::swap(arr[i], arr[parent]);
            i = parent;
        }
    }

    T* arr;
    int size, capacity;
    bool (*comp)(const T&, const T&);
};

template <typename T>
bool compare(const T& l, const T& r)
{
    return l > r;
}



int main()
{
    Heap<int> h;
    h.set_compare(compare);

    for (int i = 0; i < 100; ++i)
        h.insert(i);

    for (int i = 0; i < 100; ++i)
        std::cout << h.extract() << '\n';

    return 0;
}