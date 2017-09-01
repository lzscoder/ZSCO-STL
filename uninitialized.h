#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_
#include"type_traits.h"
#include"construct.h"
#include"iterator.h"
//写三个内存基本处理工具,为全局函数

//填充n个元素
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
	return fill_n(first, n, x);
}
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
	ForwardIterator cur = first;
	for (; n > 0; --n, ++cur)
	{
		construct(&*cur, x);
	}
}
template<class ForwardIterator,class Size,class T,class T1>
inline __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}
template<class ForwardIterator, class Size, class T>
void uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return __uninitialized_fill_n(first, n, x, value_type(first));
}



//复制函数
template<class InputIterater, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterater first, InputIterater last, ForwardIterator result, __true_type)
{
	return copy(first, last, result);
}
template<class InputIterater, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterater first, InputIterater last, ForwardIterator result, __false_type)
{
	ForwardIterator cur = result;
	for (; first != last; ++first, ++cur)
	{
		construct(&*cur, *first);
	}
	return cur;
}
template<class InputIterater, class ForwardIterator,class T>
inline ForwardIterator __uninitialized_copy(InputIterater first, InputIterater last, ForwardIterator result, T*)
{
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}
template<class InputIterater,class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterater first, InputIterater last, ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}




//填充函数
template<class ForwardIterator,class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
{
	fill(first, last, x);
}
template<class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
{
	ForwardIterator cur = first;
	for (; cur != last; ++cur)
		construct(&*cur, x);
}
template<class ForwardIterator,class T,class T1>
inline __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	__uninitialized_fill_aux(first, last, x is_POD());
}
template<class ForwardIterator,class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
	return  __uninitialized_fill(first, last, x, value_type(first));
}


#endif