#ifndef _SELECT_SORT_H_
#define _SELECT_SORT_H_

#include <functional>
#include <utility>

template <
	typename data_type,
	typename comparator_type = std::less<data_type>>
void select_sort(
	data_type *array,
	const size_t count,
	comparator_type &comparator)
{
	if (count < 2)
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

#endif
