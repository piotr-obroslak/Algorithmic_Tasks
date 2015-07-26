#include <ctime>
#include <iostream>
#include <memory>
#include <stdlib.h>

#include "insert_sort.h"

template<typename data_type>
std::unique_ptr<data_type[]> generate(const size_t count)
{
	auto test_data_wrapper = std::unique_ptr<data_type[]>(new data_type[count]);
	auto test_data_array = test_data_wrapper.get();
	
	const auto rand_limit = 5*count;
	const auto rand_offset = rand_limit/2;

	srand(time(nullptr));
	for (auto i=0; i<count; i++)
		test_data_array[i] = (rand() % rand_limit) - rand_offset;
	
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
inline void measure(dut_type &dut, data_type *array, const size_t count, comparator_type &comparator, const char *desc = nullptr)
{
	using namespace std;
	
	if (desc != nullptr)
		cout << desc;

	if (count < 20)
	{
		cout << "The input data is: ";
		for (auto i=0; i<count; i++)
			cout << array[i] << ", ";
		cout << "\n";
	}
	
	const auto t_begin = clock();
	dut(array, count, comparator);
	const auto t_end = clock();

	if (count < 20)
	{
		cout << "The output data is: ";
		for (auto i=0; i<count; i++)
			cout << array[i] << ", ";
		cout << "\n";
	}

	const auto timespan = static_cast<double>(t_end - t_begin)/CLOCKS_PER_SEC;
	cout << "it took " << timespan << " seconds.\n" << endl;
}

template<
	typename data_type,
	typename comparator_type = std::less<data_type>>
class insert_sort_dut
{
public:
	void operator() (data_type *array, const size_t count, comparator_type &comparator)
	{
		return insert_sort(array, count, comparator);
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
	
	const auto count = atoi(argv[1]);	
	auto test_data_wrapper = generate<int>(count);
	auto test_data = test_data_wrapper.get();
	
	auto _comparator = std::less<int>();
	
	{
		auto _test_data_copy_wrapper = copy_array(test_data, count);
		auto _insert_sort_dut = insert_sort_dut<int>();
		measure(_insert_sort_dut, _test_data_copy_wrapper.get(), count, _comparator, "Testing insertion sort...\n");
	}
	
	// {
		// auto _test_data_copy_wrapper = copy_array(test_data, count);
		// auto _insert_sort_dut = insert_sort_dut<int>();
		// measure(_insert_sort_dut, _test_data_copy_wrapper.get(), count, _comparator, "Testing insertion sort...\n");
	// }

	return 0;
}
