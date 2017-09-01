#ifndef _MY_CONSTRUCT_H_
#define _MY_CONSTRUCT_H_



//在这里写构造与析构函数
#include<new>
#include"type_traits.h"
//只有一个泛化的构造函数
template <class T1, class T2>
inline void construct(T1* p, const T2& value){
	new (p)T1(value);//这里调用T1::T1()
}

template<class T>
inline void construct(T* p)
{
	new(p)T();
}

template <class T>
inline void destroy(T* pointer)
{
	pointer->~T();
}



template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first < last; ++first)
		destroy(&*first);
}

template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type){}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destroy_aux(first, last, trivial_destructor());
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	__destroy(first, last, value_type(first));
}
inline void destroy(char*, char*){}
inline void destroy(wchar_t*, wchar_t*){}
inline void destroy(int*, int*){}
inline void destroy(long*, long*){}
inline void destroy(float*, float*){}
inline void destroy(double*, double*){}

#endif 