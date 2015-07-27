#ifndef _SELECT_SORT_H_
#define _SELECT_SORT_H_

#include <functional>
#include <utility>

class select_sort
{
public:
	static const char * name (void) { return "select sort"; }
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void run(data_type *array, const size_t count, comparator_type &comparator)
	{
		if (count < 1)
			return;

		for (auto i=0; i<count-1; i++)
		{
			auto min_idx = i;

			for (auto j=min_idx+1; j<count; j++)
				if (!comparator(array[min_idx], array[j]))
					min_idx = j;

			std::swap(array[i], array[min_idx]);
		}
	}
};

#endif
