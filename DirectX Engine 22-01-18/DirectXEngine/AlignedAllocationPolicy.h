#ifndef ALIGNED_ALLOCATION_POLICY_H_
#define ALIGNED_ALLOCATION_POLICY_H_

const size_t Byte8	= 8;
const size_t Byte16 = 16;

template<size_t Alignment> class AlignedAllocationPolicy
{
public:
	inline static void* operator new(size_t size)		{ return _aligned_malloc(size, Alignment); }
	inline static void operator delete(void* memory)	{ _aligned_free(memory); }
};

#endif