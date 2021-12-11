//2.1.Следующий элемент
//Дан отсортированный массив целых чисел A[0..n - 1] и массив целых чисел B[0..m - 1].
//Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
//равного или превосходящего B[i]: A[k] >= B[i].
//Если такого элемента нет, выведите n.
//Время работы поиска k для каждого элемента B[i] : O(log(k)).n, m ≤ 10000.
#include <iostream>
#include <assert.h>
#include <sstream>

int exponential_search(int* arr, int size, int key)
{
	int right = 1;
	while (right < size && arr[right] < key)
		right *= 2;

	return (right < size) ? right : size - 1;
}

int binary_search(int* arr, int left, int right, int key)
{
	while (left < right)
	{
		int center = (left + right) / 2;
		if (arr[center] < key)
			left = center + 1;
		else
			right = center;
	}
	return left;
}

void my_func(std::istream& input, std::ostream& output)
{
	int n = 0, m = 0;
	input >> n >> m;

	int* A = new int[n];
	for (int i = 0; i < n; ++i)
		input >> A[i];

	if (n > 0)
		for (int i = 0; i < m; ++i)
		{
			int b = 0;
			input >> b;

			if (i > 0)
				output << ' ';

			if (A[n - 1] < b)
				output << n;
			else
			{
				int right = exponential_search(A, n, b);
				int left = right / 2;
				output << binary_search(A, left, right, b);
			}
		}

	delete[] A;
}

void test_func()
{
	{
		std::stringstream in;
		in << "2 1" << std::endl;
		in << "1 2" << std::endl;
		in << "2" << std::endl;
		std::stringstream out;
		my_func(in, out);

		assert(out.str() == "1");
	}

	{
		std::stringstream in;
		in << "4 3" << std::endl;
		in << "2 4 5 7" << std::endl;
		in << "4 6 1" << std::endl;
		std::stringstream out;
		my_func(in, out);

		assert(out.str() == "1 3 0");
	}

	{
		std::stringstream in;
		in << "" << std::endl;
		std::stringstream out;
		my_func(in, out);

		assert(out.str() == "");
	}

	{
		std::stringstream in;
		in << "10 7" << std::endl;
		in << "1 1 1 2 2 2 3 3 3 4" << std::endl;
		in << "0 1 2 3 4 5 999" << std::endl;
		std::stringstream out;
		my_func(in, out);

		assert(out.str() == "0 0 3 6 9 10 10");
	}
	std::cout << "ALL TESTS PASSED" << std::endl;
}

int main()
{
	test_func();
	//my_func(std::cin, std::cout);

	return 0;
}