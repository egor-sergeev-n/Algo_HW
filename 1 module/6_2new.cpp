/*6. Порядковая статистика и параметры множества
Дано множество целых чисел из [0..109] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
- 10% перцентиль
- медиана
- 90% перцентиль
Требования:
- К дополнительной памяти: O(n).
- Среднее время работы: O(n)
- Должна быть отдельно выделенная функция partition.
- Рекурсия запрещена.
- Решение должно поддерживать передачу функции сравнения снаружи.*/
#include <iostream>
#include <assert.h>
#include <sstream>
#include <time.h>

int swap_values(int* arr, size_t l, size_t r, bool (*comp) (int, int) = [](int lhs, int rhs) { return lhs < rhs; }) {
    size_t c = (l + r) / 2;
    r--;
    if (comp(arr[l], arr[c])) {
        if (comp(arr[r], arr[c])) {
            if (comp(arr[l], arr[r]))
                std::swap(arr[c], arr[r]);
            else {
                std::swap(arr[c], arr[r]);
                std::swap(arr[l], arr[c]);
            }
        }
    }
    else if (comp(arr[r], arr[c])) 
        std::swap(arr[l], arr[r]);
    else {
        if (comp(arr[l], arr[r]))
            std::swap(arr[l], arr[c]);
        else {
            std::swap(arr[l], arr[c]);
            std::swap(arr[c], arr[r]);
        }
    }

    std::swap(arr[c], arr[r - 1]);
    return arr[r - 1];
}

size_t partition(int* arr, size_t left, size_t right, bool (*comp) (int, int) = [](int lhs, int rhs) { return lhs < rhs; }) {
    int d = right - left;
    if (d <= 2) {
        if (d == 1)
            return left;
        if (comp(arr[left + 1], arr[left]))
            std::swap(arr[left], arr[left + 1]);
        return left;
    }

    int pivot = swap_values(arr, left, right, comp);
    size_t i = right - 3, j = i;
    while (true) {
        if (comp(pivot, arr[j])) {
            if (i != j)
                std::swap(arr[i], arr[j]);
            i--;
        }
        if (j == left)
            break;
        j--;
    }
    std::swap(arr[i + 1], arr[right - 2]);
    return i + 1;
}

void kthElement(int* arr, size_t size, size_t k, bool (*comp) (int, int) = [](int lhs, int rhs) { return lhs < rhs; }) {
    size_t l = 0, r = size;

    while (true) {
        size_t m = partition(arr, l, r, comp);

        if (m == k)
            return;
        if (m > k)
            r = m;
        else
            l = m + 1;
    }
}

void processRequest(std::istream& in, std::ostream& out) {
    size_t n = 0;
    in >> n;
    int* x = new int[n];
    for (size_t i = 0; i < n; ++i)
        in >> x[i];
    size_t n_10 = n / 10;
    size_t n_50 = n / 2;
    size_t n_90 = 9 * n / 10;
    kthElement(x, n, n_10);
    kthElement(x, n, n_50);
    kthElement(x, n, n_90);
    out << x[n_10] << std::endl;
    out << x[n_50] << std::endl;
    out << x[n_90] << std::endl;
    delete[] x;
}

void testPartition() {
    {
        int x[] = { 1, 2, 3 };
        size_t m = partition(x, 0, 3);
        assert(m == 1);
        assert(x[0] == 1);
        assert(x[1] == 2);
        assert(x[2] == 3);
    }
    {
        int x[] = { 1, 5, 3, 6, 2 };
        size_t m = partition(x, 0, 5);
        assert(m == 1);
        assert(x[m] == 2);
    }
    std::cout << "Partition tests passed\n";
}

void testKthElement() {
    {
        int x[] = { 1, 9, -1 };
        kthElement(x, 3, 0);
        assert(x[0] == -1);
    }
    {
        int x[] = { 1, 9, -1 };
        kthElement(x, 3, 1);
        assert(x[1] == 1);
    }
    {
        int x[] = { 1, 9, -1 };
        kthElement(x, 3, 2);
        assert(x[2] == 9);
    }
    std::cout << "kth element tests passed\n";
}

void testRequest() {
    {
        std::stringstream in, out;
        in << "10\n1 2 3 4 5 6 7 8 9 10";
        processRequest(in, out);
        assert(out.str() == "2\n6\n10\n");
    }
    {
        std::stringstream in, out;
        in << "1\n5";
        processRequest(in, out);
        assert(out.str() == "5\n5\n5\n");
    }
    {
        std::stringstream in, out;
        in << "20\n2 4 6 8 10 12 14 16 18 20 1 3 5 7 9 11 13 15 17 19";
        processRequest(in, out);
        assert(out.str() == "3\n11\n19\n");
    }
    std::cout << "Request tests passed\n";
}

int main() {
    /*testPartition();
    testKthElement();
    testRequest();
    testBig();*/
    processRequest(std::cin, std::cout);
    return 0;
}