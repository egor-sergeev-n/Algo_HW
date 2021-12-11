//4.1.Слияние массивов
//Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
//Сначала вводится количество массивов K.
//Затем по очереди размер каждого массива и элементы массива.
//Каждый массив упорядочен по возрастанию.
#include <iostream>
#include <assert.h>
#include <sstream>
#define INCREASE_FACTOR 2

template<class T>
class Heap
{
public:
	Heap(bool (*cmp) (const T&, const T&) = [](const T& lhs, const T& rhs) { return lhs < rhs; }) :
		arr(nullptr),
		size(0),
		capacity(0),
		comp(cmp) {};

	Heap(T* a, size_t s, bool (*cmp) (const T&, const T&) = [](const T& lhs, const T& rhs) { return lhs < rhs; }) :
		arr(a),
		size(s),
		capacity(s),
		comp(cmp)
	{
		build_heap();
	};

	~Heap()
	{
		delete[] arr;
	}

	Heap(const Heap&) = delete;
	Heap(Heap&&) = delete;

	Heap& operator=(const Heap&) = delete;
	Heap& operator=(Heap&&) = delete;

	const T& top() const
	{
		assert(!empty());
		return arr[0];
	}

	void pop()
	{
		assert(!empty());

		size--;
		arr[0] = arr[size];

		if (!empty())
			sift_down(0);
	}

	void push(const T& elem)
	{
		if (fool())
		{
			if (capacity == 0)
			{
				capacity = 1;
				arr = new T[1];
			}
			else
			{
				capacity *= INCREASE_FACTOR;
				T* new_arr = new T[capacity];
				for (size_t i = 0; i < size; ++i)
					new_arr[i] = arr[i];

				delete[] arr;
				arr = new_arr;
			}
		}

		arr[size] = elem;
		sift_up(size);
		++size;
	}

	size_t get_size() const
	{
		return size;
	}

	bool empty() const
	{
		return (size == 0);
	}

private:
	bool fool() const
	{
		return (size == capacity);
	}

	void build_heap()
	{
		if (!empty())
		{
			for (size_t i = (size - 1) / 2; i > 0; --i)
				sift_down(i);
			sift_down(0);
		}
	}

	void sift_up(size_t idx)
	{
		while (idx > 0)
		{
			size_t parent = (idx - 1) / 2;
			if (comp(arr[parent], arr[idx]))
				return;

			std::swap(arr[idx], arr[parent]);
			idx = parent;
		}
	}

	void sift_down(size_t idx)
	{
		while (true)
		{
			size_t left = 2 * idx + 1;
			size_t right = 2 * idx + 2;

			size_t largest = idx;
			if (left < size && comp(arr[left], arr[idx]))
				largest = left;
			if (right < size && comp(arr[right], arr[largest]))
				largest = right;

			if (largest == idx)
				return;

			std::swap(arr[idx], arr[largest]);
			idx = largest;
		}
	}

	T* arr;
	size_t size, capacity;
	bool (*comp) (const T&, const T&);
};

template<class T>
struct ArrayStruct
{
	T val;
	size_t num_of_array, idx;

	ArrayStruct() : val(T()), num_of_array(0), idx(0) {}
	ArrayStruct(const T& v, size_t n) : val(v), num_of_array(n), idx(0) {}
};


template<class T>
void merge_k_arrays(T** arr, size_t* sizes, size_t k, T* arr_to_sort, size_t n)
{
	ArrayStruct<T>* first_vals = new ArrayStruct<T>[k];
	for (size_t i = 0; i < k; ++i)
	{
		first_vals[i].val = arr[i][0];
		first_vals[i].num_of_array = i;
	}

	Heap<ArrayStruct<T>> heap(first_vals, k, [](const ArrayStruct<T>& lhs, const ArrayStruct<T>& rhs)
	{return lhs.val < rhs.val; });

	for (size_t i = 0; i < n; ++i)
	{
		ArrayStruct<T> min = heap.top();
		arr_to_sort[i] = min.val;
		heap.pop();

		min.idx++;
		if (min.idx < sizes[min.num_of_array])
		{
			min.val = arr[min.num_of_array][min.idx];
			heap.push(min);
		}
	}
}


void merge_from_stream(std::istream& in, std::ostream& out)
{
	size_t k = 0, n = 0;
	in >> k;

	int** arrays = new int* [k];
	size_t* sizes = new size_t[k];
	for (size_t i = 0; i < k; ++i)
	{
		in >> sizes[i];
		n += sizes[i];
		arrays[i] = new int[sizes[i]];

		for (size_t j = 0; j < sizes[i]; ++j)
			in >> arrays[i][j];
	}

	int* sorted = new int[n];
	merge_k_arrays(arrays, sizes, k, sorted, n);

	if (n > 0)
		out << sorted[0];
	for (size_t i = 1; i < n; ++i)
		out << ' ' << sorted[i];

	for (size_t i = 0; i < k; ++i)
		delete[] arrays[i];
	delete[] arrays;
	delete[] sizes;
	delete[] sorted;
}



void test_Heap()
{
	{
		Heap<float> h;
		assert(h.empty());

		h.push(1.0);
		h.push(2.0);
		h.push(3.0);

		assert(h.top() == 1.0);
		h.pop();
		assert(h.top() == 2.0);
		h.pop();
		assert(h.top() == 3.0);
		h.pop();

		assert(h.empty());
	}

	{
		double* arr = new double[5];
		arr[0] = 1.0;
		arr[1] = 3.0;
		arr[2] = 3.0;
		arr[3] = -7.0;
		arr[4] = 2.0;
		Heap<double> h(arr, 5);
		assert(h.get_size() == 5);

		assert(h.top() == -7.0);
		h.pop();
		assert(h.top() == 1.0);
		h.pop();
		assert(h.top() == 2.0);
		h.pop();
		assert(h.top() == 3.0);
		h.pop();
		assert(h.top() == 3.0);
		h.pop();

		assert(h.empty());
	}
	std::cout << "Heap tests passed\n";
}


void test_merge()
{
	{
		std::stringstream in;
		in << "3\n";
		in << "1\n";
		in << "6\n";
		in << "2\n";
		in << "50 90\n";
		in << "3\n";
		in << "1 10 70\n";

		std::stringstream out;
		merge_from_stream(in, out);
		assert(out.str() == "1 6 10 50 70 90");
	}

	{
		std::stringstream in;
		in << "3\n";
		in << "5\n";
		in << "1 2 3 4 5\n";
		in << "5\n";
		in << "3 5 7 8 9\n";
		in << "5\n";
		in << "50 100 150 200 500\n";

		std::stringstream out;
		merge_from_stream(in, out);
		assert(out.str() == "1 2 3 3 4 5 5 7 8 9 50 100 150 200 500");
	}

	{
		std::stringstream in;
		in << "0\n";

		std::stringstream out;
		merge_from_stream(in, out);
		assert(out.str() == "");
	}

	{
		std::stringstream in;
		in << "1\n";
		in << "5\n";
		in << "1 2 3 4 5\n";

		std::stringstream out;
		merge_from_stream(in, out);
		assert(out.str() == "1 2 3 4 5");
	}

	{
		std::stringstream in;
		in << "3\n";
		in << "1\n";
		in << "1\n";
		in << "1\n";
		in << "2\n";
		in << "1\n";
		in << "3\n";

		std::stringstream out;
		merge_from_stream(in, out);
		assert(out.str() == "1 2 3");
	}

	std::cout << "Merge tests passed\n";
}




int main()
{
	test_Heap();
	test_merge();
	//merge_from_stream(std::cin, std::cout);

	return 0;
}