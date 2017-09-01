#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include"alloc.h"
template<class T, class Alloc>
class simple_alloc{
public:
	typedef size_t     size_type;
	typedef ptrdiff_t  difference_type;
	typedef T*       pointer;
	typedef const T* const_pointer;
	typedef T&       reference;
	typedef const T& const_reference;
	typedef T        value_type;
	static T* allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T));
	}
	static T* allocate(void)
	{
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n)
	{
		if (n != 0) Alloc::deallocate(p, sizeof(T)*n);
	}
	static void deallocate(T* p)
	{
		Alloc::deallcate(p, sizeof(T));
	}
};
#endif