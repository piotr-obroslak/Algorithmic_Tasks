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
		build_heap(array, count,cmp);
		
		auto heap_size = count;
		for (auto i = count-1; i>0; i--)
		{
			std::swap(array[0], array[i]);
			restore_heap(array, 1, --heap_size, cmp);
		}
	}
	
protected:
	static size_t array_index(const size_t index)
	{
		return index -1;
	}
	
	static size_t left_heap_child_index(const size_t index)
	{
		return index << 1;
	}
	
	static size_t right_heap_child_index(const size_t index)
	{
		return left_heap_child_index(index) + 1;
	}
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void restore_heap(data_type *array, const size_t index, const size_t heap_size, comparator_type &cmp)
	{
		if (index > heap_size)
			return;
		
		auto most_deheapifying_index = index;
		
		const auto left_child_index = left_heap_child_index(index);
		if (left_child_index <= heap_size)
		{
			if (!cmp(array[array_index(left_child_index)], array[array_index(most_deheapifying_index)]))
				most_deheapifying_index = left_child_index;
		}
		
		const auto right_child_index = right_heap_child_index(index);
		if (right_child_index <= heap_size)
		{
			if (!cmp(array[array_index(right_child_index)], array[array_index(most_deheapifying_index)]))
				most_deheapifying_index = right_child_index;
		}

		if (most_deheapifying_index != index)
		{
			std::swap(array[array_index(index)], array[array_index(most_deheapifying_index)]);
			restore_heap(array, most_deheapifying_index, heap_size, cmp);
		}
	}
	
	template<typename data_type, typename comparator_type = std::less<data_type>>
	static void build_heap(data_type *array, const size_t count, comparator_type &cmp)
	{
		const auto mid = count >> 1;
		
		for (auto i=mid; i>0 ; i--)
			restore_heap(array, i, count, cmp);
	}
};

#endif
