#pragma once
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <windows.h>
#include <math.h>

template<class _Ty, class _Alloc = allocator<_Ty>> class List
{
public:
	std::vector<_Ty, _Alloc> data;

public:
	void Add(const _Ty& element)
	{
		data.push_back(element);
	}

	void RemoveAt(size_t index)
	{
		if (index >= data.size()) throw std::out_of_range("Index out of range");
		data.erase(data.begin() + index);
	}

	void Clear()
	{
		data.clear();
	}

	bool Contains(const _Ty& element) const
	{
		return std::find(data.begin(), data.end(), element) != data.end();
	}

	int IndexOf(const _Ty& element) const
	{
		auto it = std::find(data.begin(), data.end(), element);
		return (it != data.end()) ? std::distance(data.begin(), it) : -1;
	}

	void Insert(size_t index, const _Ty& element)
	{
		if (index > data.size()) throw std::out_of_range("Index out of range");
		data.insert(data.begin() + index, element);
	}

	bool Remove(const _Ty& element)
	{
		auto it = std::find(data.begin(), data.end(), element);
		if (it != data.end())
		{
			data.erase(it);
			return true;
		}
		return false;
	}

	size_t Count() const
	{
		return data.size();
	}

	bool IsEmpty() const
	{
		return data.empty();
	}

	_Ty& operator[](size_t index)
	{
		if (index >= data.size())
			throw std::out_of_range("Index out of range");

		return data[index];
	}
};
