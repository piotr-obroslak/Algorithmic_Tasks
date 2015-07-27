#ifndef _MERGE_SORT_H_
#define _MERGE_SORT_H_

#include <functional>
#include <memory>
#include <utility>

class merge_sort
{
public:
	static const char * name (void) { return "merge sort"; }
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void run(data_type *array, const size_t count, comparator_type &comparator)
	{
		return recursive_run(array, 0, count, comparator);
	}
	
protected:
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void recursive_run(
		data_type *array,
		const size_t p,
		const size_t r,
		comparator_type &comparator)
	{
		if (r-p >= 2)
		{
			auto q = (p+r)/2;
			recursive_run(array, p, q, comparator);
			recursive_run(array, q, r, comparator);
			
			merge(array, p, q, r, comparator);
		}
	}
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void merge(
		data_type *array,
		const size_t p,
		const size_t q,
		const size_t r,
		comparator_type &comparator)
	{
		const auto array_left_size = q-p;
		auto array_left = std::unique_ptr<data_type[]>(new data_type[array_left_size]);
		for (auto k=0; k<array_left_size; k++)
			array_left[k] = array[p+k];

		const auto array_right_size = r-q;
		auto array_right = std::unique_ptr<data_type[]>(new data_type[array_right_size]);
		for (auto k=0; k<array_right_size; k++)
			array_right[k] = array[q+k];
		
		auto i=0;
		auto j=0;
		
		for (auto k=p; k<r; k++)
		{
			if (i<array_left_size && j<array_right_size)
			{
				if (comparator(array_left[i], array_right[j]))
					array[k] = array_left[i++];
				else
					array[k] = array_right[j++];
			}
			else if (i<array_left_size)
			{
				array[k] = array_left[i++];
			}
			else if (j<array_right_size)
			{
				array[k] = array_right[j++];
			}
		}
	}
};

#endif
