#ifndef _QUICK_SORT_H_
#define _QUICK_SORT_H_

#include <functional>
#include <utility>

class quick_sort
{
public:
	static const char * name (void) { return "quick sort"; }
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void run(data_type *array, const size_t count, comparator_type &comparator)
	{
	}
};

#endif
