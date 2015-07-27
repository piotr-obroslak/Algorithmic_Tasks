#ifndef _BUBBLE_SORT_H_
#define _BUBBLE_SORT_H_

#include <functional>
#include <utility>

class bubble_sort
{
public:
	static const char * name (void) { return "bubble sort"; }
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void run(data_type *array, const size_t count, comparator_type &comparator)
	{
		for (auto i=0; i<count; i++)
			for (auto j=count-1; j>i; j--)
				if (!comparator(array[j-1], array[j]))
					std::swap(array[j-1], array[j]);
	}
};

#endif
