#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_
#define  __STL_TEMPLATE_NULL template<>
//char, signed char,unsigned char,short,unsigned short
//int,unsinged int,long, unsinged long,float,double,,long double
struct __true_type{};
struct __false_type{};
template<class type>
struct __type_traits
{
	typedef __ture_type this_dummy_member_must_be_first;

	typedef __false_type has_trival_default_constructor;
	typedef __false_type has_trival_copy_constructor;
	typedef __false_type has_trival_assignment_operator;
	typedef __false_type has_trival_destructor;
	typedef __false_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<char>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<signed char>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<unsigned char>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<unsigned short>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<short>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<int>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<unsigned int>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<long>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<unsigned long>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<float>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<double>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
__STL_TEMPLATE_NULL struct __type_traits<long double>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
template<class T>
struct __type_traits<T*>
{
	typedef __true_type has_trival_default_constructor;
	typedef __true_type has_trival_copy_constructor;
	typedef __true_type has_trival_assignment_operator;
	typedef __true_type has_trival_destructor;
	typedef __true_type is_POD_type;
};
#endif