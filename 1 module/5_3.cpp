//5.3. Закраска прямой
//На числовой прямой окрасили N отрезков.
//Известны координаты левого и правого концов каждого отрезка [Li, Ri]. Найти длину окрашенной части числовой прямой.
//N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 109].

#include <iostream>
#include <assert.h>
#include <cstring>
#include <sstream>

template<typename T>
struct DefaultComparator{
    bool operator() (const T& lhs, const T& rhs){
        return lhs < rhs;
    }
};

template<typename T, typename Comparator = DefaultComparator<T>>
void merge(T *p1, size_t len1, T *p2, size_t len2, T *c,  Comparator comp = Comparator()){
    size_t i = 0, i1 = 0, i2 = 0;

    while (i1 < len1 && i2 < len2)
        c[i++] = comp(p1[i1], p2[i2]) ? p1[i1++] : p2[i2++];
    
    while(i1 < len1)
        c[i++] = p1[i1++];
    while(i2 < len2)
        c[i++] = p2[i2++];
}

template<typename T, typename Comparator = DefaultComparator<T>>
void mergeSort(T *arr, size_t len, Comparator comp = Comparator()){
    // в данном случае мы можем использовать рекурсию,
    // т.к. её глубина точно не превысит 1000 (иначе было бы больше,
    // чем 2^1000 элементов в массиве)
    if (len <= 1)
        return;
    
    size_t first_len = len / 2;
    size_t second_len = len - first_len;
    mergeSort(arr, first_len, comp);
    mergeSort(arr + first_len, second_len, comp);
    T *c = new T[len];
    merge(arr, first_len, arr + first_len, second_len, c, comp);
    memcpy(arr, c, sizeof(T) * len);
    delete[] c;
}


struct Dot{
    int val;
    bool end;
};

void printDots(Dot *dots, size_t size){
    if (size == 0)
        return;
    
    std::cout << dots[0].val;
    for (size_t i = 1; i < size; ++i)
        std::cout << ' ' << dots[i].val;
    std::cout << std::endl;
}

int findLength(Dot *dots, size_t size){
    mergeSort(dots, size, [](const Dot &lhs, const Dot &rhs) { return lhs.val < rhs.val; });
    int length = 0, n = 0;
    int start = dots[0].val;
    for (int i = 0; i < size; ++i){
        if (dots[i].end){
            if (--n == 0)
                length += dots[i].val - start;
        }
        else if (n++ == 0)
            start = dots[i].val;
    }
    return length;
}

void processRequest(std::istream& in, std::ostream& out){
    int n = 0;
    in >> n;
    Dot *dots = new Dot[2 * n];
    int j = 0;
    for (int i = 0; i < n; ++i){
        in >> dots[j].val;
        dots[j++].end = false;

        in >> dots[j].val;
        dots[j++].end = true;
    }

    int length = findLength(dots, 2 * n);
    out << length;

    delete[] dots;
}

void mergeTest(){
    {
        //проверка на отсутствие ошибок запуска такого случая
        int *p1, *p2, *c;
        merge(p1, 0, p2, 0, c);
    }
    {
        int p1[] = {1};
        int p2[] = {2};
        int c[2];
        merge(p1, 1, p2, 1, c);
        assert(c[0] = 1 && c[1] == 2);

        merge(p1, 1, p2, 1, c, [](int l, int r) { return l > r; });
        assert(c[0] = 2 && c[1] == 1);
    }
    {
        int p1[] = {1, 4, 5, 8};
        int p2[] = {2, 3, 6, 7};
        int c[8];
        merge(p1, 4, p2, 4, c);
        for (int i = 0; i < 8; ++i)
            assert(c[i] == i + 1);
    }
    {
        double p1[] = {8, 5, 4, 1};
        double p2[] = {7, 6, 3, 2};
        double c[8];
        merge(p1, 4, p2, 4, c, [](double l, double r){ return l > r; });
        for (int i = 0; i < 8; ++i)
            assert(c[i] == 8 - i);
    }
    std::cout << "merge tests passed\n";
}

void mergeSortTest(){
    {
        float *x;
        mergeSort(x, 0);
    }
    {
        float x[] = {3.1f, 2.9f, 5.01f};
        mergeSort(x, 3);
        assert(x[0] == 2.9f);
        assert(x[1] == 3.1f);
        assert(x[2] == 5.01f);
    }
    {
        char x[] = {'b','a','f','d','e','c'};
        mergeSort(x, 6);
        assert(x[0] == 'a');
        assert(x[1] == 'b');
        assert(x[2] == 'c');
        assert(x[3] == 'd');
        assert(x[4] == 'e');
        assert(x[5] == 'f');
    }
    std::cout << "mergeSort tests passed\n";
}

void testFindLength(){
    {
        Dot dots[6] = {{1, false}, {4, true},
                       {7, false}, {8, true},
                       {2, false}, {5, true}};
        assert(findLength(dots, 6) == 5);
    }
    {
        Dot dots[6] = {{1, false}, {4, true},
                       {6, false}, {8, true},
                       {2, false}, {5, true}};
        assert(findLength(dots, 6) == 6);
    }
    {
        Dot dots[6] = {{1, false}, {4, true},
                       {6, false}, {8, true},
                       {2, false}, {4, true}};
        assert(findLength(dots, 6) == 5);
    }
    std::cout << "findLength tests passed\n";
}

void testRequest(){
    {
        std::stringstream in;
        in << "1\n-2 2";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "4");
    }
    {
        std::stringstream in;
        in << "3\n1 4\n7 8\n2 5";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "5");
    }
    {
        std::stringstream in;
        in << "3\n1 4\n6 8\n2 5";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "6");
    }
    {
        std::stringstream in;
        in << "3\n1 4\n6 8\n2 4";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "5");
    }
    std::cout << "request tests passed\n";
}

int main(){
    /*mergeTest();
    mergeSortTest();
    testFindLength();
    testRequest();*/
    processRequest(std::cin, std::cout);


    return 0;
}