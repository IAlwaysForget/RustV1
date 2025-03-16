#ifndef DYANMICARRAY_HPP
#define DYANMICARRAY_HPP

template<typename t>
class DynamicArray
{
public:
	int GetSize();
	t GetIndex(int index);
	std::vector<t> GetData();
private:
	struct
	{
		unsigned long long ptr;
		struct MemLabelId
		{
			int identifier;
		}label;
		char padding[4];
		unsigned long long size;
		unsigned long long capacity;
	}dynamic_array_detail;
};

template<typename t> int DynamicArray<t>::GetSize()
{
	return dynamic_array_detail.size;
}

template<typename t> t DynamicArray<t>::GetIndex(int index)
{
	return mem.Read<t>(dynamic_array_detail.ptr + (index * sizeof(t)));
}

template<typename t> std::vector<t> DynamicArray<t>::GetData()
{
	unsigned long long size = GetSize();
	if (size < 0 || size > 2000)
		return {};

	void* data = malloc(size * sizeof(t));
	mem.Read(dynamic_array_detail.ptr, data, size * sizeof(t));

	t* data_ptr = static_cast<t*>(data);
	std::vector<t> result(data_ptr, data_ptr + size);
	free(data);

	return result;
}

#endif // !DYANMICARRAY_HPP