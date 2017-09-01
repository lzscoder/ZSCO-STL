#ifndef _ALGOBASE_H_
#define _ALGOBASE_H_

//fill
template<class ForwardIterator,class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
	while (; first != last; fist++)
		*first = value;
}

//fill_n
template<class OutputIterator, class Size,class T>
OutputIterator fill(OutputIterator first, Size n, const T& value)
{
	for (; n > 0; --n; ++first)
		*first = value;
	return  first;
}

//写一个零时的copy用于uninitialized.h
//可能有错误
template<class InputIterater,class ForwardIterator>
void copy(InputIterater first, InputIterater last, ForwardIterator result)
{
	for (; first != last; first++, result++)
		*first = *result;
	return first;
}

#endif