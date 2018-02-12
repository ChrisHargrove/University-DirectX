#pragma once

const size_t BYTE_8		= 8;
const size_t BYTE_16	= 16;

template<size_t Alignment> class AlignedAllocationPolicy
{
public:
	inline static void* operator new(size_t size)		{ return _aligned_malloc(size, Alignment); }
	inline static void operator delete(void* memory)	{ _aligned_free(memory); }
};