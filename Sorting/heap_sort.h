#ifndef _HEAP_SORT_H_
#define _HEAP_SORT_H_

#include <functional>
#include <utility>

class heap_sort
{
public:
	static const char * name(void) { return "heap sort"; }
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void run(data_type *array, const size_t count, comparator_type &cmp)
	{
	}
};

#endif
