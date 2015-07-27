#ifndef _MERGE_SORT_H_
#define _MERGE_SORT_H_

#include <functional>
#include <utility>

class merge_sort
{
public:
	static const char * name (void) { return "merge sort"; }
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void run(data_type *array, const size_t count, comparator_type &comparator)
	{
	}
};

#endif
