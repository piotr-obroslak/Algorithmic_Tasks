#ifndef _QUICK_SORT_H_
#define _QUICK_SORT_H_

#include <iostream>

#include <functional>
#include <utility>

class quick_sort
{
public:
	static const char * name (void) { return "quick sort"; }
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void run(data_type *array, const size_t count, comparator_type &comparator)
	{
		return recursive_run(array, 0, count, comparator);
	}
	
protected:
	template<typename data_type, typename comparator_type>
	static void recursive_run(
		data_type *array,
		const size_t p,
		const size_t r,
		comparator_type &comparator)
	{
		if (p >= r)
			return;

		const auto q = partition(array, p, r, comparator);
		recursive_run(array, p, q, comparator);
		recursive_run(array, q + 1, r, comparator);
	}
	
	template<typename data_type, typename comparator_type>
	static size_t partition(
		data_type *array,
		const size_t p,
		const size_t r,
		comparator_type &comparator)
	{
		const auto partitioner = array[p];
		auto i=r;
		for (auto j=r-1; j>p; j--)
		{
			if (!comparator(array[j], partitioner))
			{
				i=i-1;
				std::swap(array[i], array[j]);
			}
		}
		std::swap(array[i-1], array[p]);

		return i-1;
	}
};

#endif
