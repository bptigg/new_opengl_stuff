#pragma once

#include <vector>

namespace Utility
{
	template<typename t>
	int partition(std::vector<t> vec, int low, int high)
	{
		t pivot = vec[high];
		int i = low - 1;

		for (int j = low; j <= high - 1; j++)
		{
			if (vec[j] < pivot)
			{
				i++;
				std::swap(vec[i], vec[j]);
			}
		}
		std::swap(vec[i + 1], vec[high]);
		return (i + 1);
	}

	template<typename t>
	void quicksort(std::vector<t> vec, int low, int high)
	{
		if (low < high)
		{
			int pi = partition(vec, low, high);
			quicksort(vec, low, pi - 1);
			quicksort(vec, pi + 1, high);
		}
	}


	template<typename t>
	bool find_in_array(t* list, uint32_t size, t value)
	{
		t* ArraryPtr = list;
		for (uint32_t i = 0; i < size; i++)
		{
			if (*ArraryPtr == value)
			{
				return true;
			}
			ArraryPtr++;
		}
		return false;
	}

	template<typename t>
	bool find_in_array(std::vector<t> vec, t value)
	{
		for (uint32_t i = 0; i < vec.size(); i++)
		{
			if (vec[i] == value)
			{
				return true;
			}
		}
		return false;
	}

}