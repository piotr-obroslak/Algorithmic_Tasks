#include <ctime>
#include <iostream>
#include <memory>
#include <stdlib.h>

#include "bubble_sort.h"
#include "heap_sort.h"
#include "insert_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include "select_sort.h"

template<typename data_type>
std::unique_ptr<data_type[]> generate_random_array(const size_t count)
{
	auto test_data_wrapper = std::unique_ptr<data_type[]>(new data_type[count]);
	auto test_data_array = test_data_wrapper.get();

	const auto rand_limit = 5*count;
	const auto rand_offset = rand_limit/2;

	srand(time(nullptr));
	for (auto i=0; i<count; i++)
		test_data_array[i] = static_cast<data_type>((rand() % rand_limit) - rand_offset);
	
	return test_data_wrapper;
}

template<typename data_type>
std::unique_ptr<data_type[]> copy_array(const data_type *array, const size_t count)
{
	auto test_data_copy_wrapper = std::unique_ptr<data_type[]>(new data_type[count]);
	auto test_data_copy = test_data_copy_wrapper.get();

	for (auto i=0; i<count; i++)
		test_data_copy[i] = array[i];

	return test_data_copy_wrapper;
}

template<typename data_type, typename comparator_type = std::less<data_type>, typename dut_type>
inline void measure(dut_type &dut, data_type *array, const size_t count, comparator_type &comparator)
{
	using namespace std;

	auto print_array_content = [](const data_type *array, const size_t count, const char *desc)
	{
		if (count > 20)
			return;

		cout << desc;
		for (auto i=0; i<count; i++)
			cout << array[i] << ", ";
		cout << "\n";
	};

	cout << "Testing " << dut.desc() << "\n";

	print_array_content(array, count, "The input data is: ");

	const auto t_begin = clock();
	dut.run(array, count, comparator);
	const auto t_end = clock();

	print_array_content(array, count, "The output data is: ");

	const auto timespan = static_cast<double>(t_end - t_begin)/CLOCKS_PER_SEC;
	cout << "it took " << timespan << " seconds.\n" << endl;
}

template<
	typename algorithm_type>
class sort_test
{
public:
	const char * desc() const
	{
		return algorithm_type::name();
	}
	
	template<typename data_type, typename comparator_type>
	void run(data_type *array, const size_t count, comparator_type &comparator) const
	{
		return algorithm_type::run(array, count, comparator);
	}
};


int main(int argc, char*argv[])
{
	using namespace std;
	
	if (argc < 2)
	{
		cout << "Please specify the number of elements!\n" << endl;
		return 1;
	}
	
	const auto _count = atoi(argv[1]);	
	auto _test_data_wrapper = generate_random_array<int>(_count);
	auto _test_data = _test_data_wrapper.get();
	
	auto _comparator = std::less<int>();
	
	{
		auto _test_data_copy_wrapper = copy_array(_test_data, _count);
		auto _sort_test = sort_test<insert_sort>();
		measure(_sort_test, _test_data_copy_wrapper.get(), _count, _comparator);
	}

	{
		auto _test_data_copy_wrapper = copy_array(_test_data, _count);
		auto _sort_test = sort_test<bubble_sort>();
		measure(_sort_test, _test_data_copy_wrapper.get(), _count, _comparator);
	}

	{
		auto _test_data_copy_wrapper = copy_array(_test_data, _count);
		auto _sort_test = sort_test<select_sort>();
		measure(_sort_test, _test_data_copy_wrapper.get(), _count, _comparator);
	}

	{
		auto _test_data_copy_wrapper = copy_array(_test_data, _count);
		auto _sort_test = sort_test<heap_sort>();
		measure(_sort_test, _test_data_copy_wrapper.get(), _count, _comparator);
	}

	return 0;
}
