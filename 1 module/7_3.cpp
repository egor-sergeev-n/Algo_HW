//7.3.Binary MSD для long long
//Дан массив неотрицательных целых 64 - разрядных чисел.
//Количество чисел не больше 1000000. Отсортировать массив методом MSD по битам(бинарный QuickSort).
#include <iostream>
#include <assert.h>
#include <sstream>

int nth_bit_zero(unsigned long long int num, int bit) {
    return (num & (1ull << bit)) == 0;
}

void binaryQuickSort(unsigned long long int *arr, int left, int right, int bit = 63) {
    if (right <= left || bit < 0)
        return;

    int i = left;
    int j = right;

    while (j != i) {
        for (; nth_bit_zero(arr[i], bit) && i < j; ++i);
        for (; !nth_bit_zero(arr[j], bit) && i < j; --j);
        std::swap(arr[i], arr[j]);
    }

    if (nth_bit_zero(arr[right], bit)) j++;
    binaryQuickSort(arr, left, j - 1, bit - 1);
    binaryQuickSort(arr, j, right, bit - 1);
}

void processRequest(std::istream& in, std::ostream& out) {
    int size = 0;
    in >> size;
    unsigned long long int* arr = new unsigned long long int[size];
    for (int i = 0; i < size; ++i)
        in >> arr[i];
    binaryQuickSort(arr, 0, size - 1);
    if (size > 0)
        out << arr[0];
    for (int i = 1; i < size; ++i)
        out << ' ' << arr[i];
}

void testSort() {
    {
        unsigned long long int x[] = {0, 1, 9, 2, 8, 3, 7, 4, 6, 5};
        binaryQuickSort(x, 0, 9);
        for (int i = 0; i <= 9; ++i)
            assert(x[i] == i);
    }
    {
        unsigned long long int x[] = { 2, 1 };
        binaryQuickSort(x, 0, 1);
        assert(x[0] == 1);
        assert(x[1] == 2);
    }
    {
        unsigned long long int x[] = { 4, 1000000, 7 };
        binaryQuickSort(x, 0, 2);
        assert(x[0] == 4);
        assert(x[1] == 7);
        assert(x[2] == 1000000);
    }
    std::cout << "Sort tests passed\n";
}

void testRequest() {
    {
        std::stringstream in, out;
        in << "10\n0 1 9 2 8 3 7 4 6 5";
        processRequest(in, out);
        assert(out.str() == "0 1 2 3 4 5 6 7 8 9");
    }
    {
        std::stringstream in, out;
        in << "2\n2 1";
        processRequest(in, out);
        assert(out.str() == "1 2");
    }
    {
        std::stringstream in, out;
        in << "3\n4 1000000 7";
        processRequest(in, out);
        assert(out.str() == "4 7 1000000");
    }
    std::cout << "Request tests passed\n";
}

int main()
{
    //testSort();
    //testRequest();
    processRequest(std::cin, std::cout);

    return 0;
}
