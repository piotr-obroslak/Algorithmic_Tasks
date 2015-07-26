#ifndef _INSERT_SORT_H_
#define _INSERT_SORT_H_

#include <functional>
#include <utility>

template<
	typename data_type,
	typename comparator_type = std::less<data_type>>
inline void insert_sort(
	data_type *array,
	const size_t count,
	comparator_type &comparator)
{
	for (auto i=1; i<count; i++)
		for (auto j=i; j>0 ; j--)
			if (comparator(array[j],array[j-1]))
				std::swap(array[j], array[j-1]);
}

#endif
