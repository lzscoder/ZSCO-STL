#ifndef _ALLOC_H_
#define _ALLOC_H_
#include<stdlib.h>
namespace zsco{
//第一级配置器
//这个版本就不用template编程
	#if 0  
	#   include <new>  
	#   define __THROW_BAD_ALLOC throw bad_alloc  
	#elif !defined(__THROW_BAD_ALLOC)//定义内存申请出错处理  
	#   include <iostream>  
	#   define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)  
	#endif 


	class __malloc_alloc_template{
	private:
		//要有处理内存不足的函数
		static void *oom_malloc(size_t);
		static void *oom_realloc(void*, size_t);
		static void(*__malloc_alloc_oom_handler)();//这是一个函数指针，静态成员变量初始化为0

	public:
		static void* allocate(size_t n)
		{
			void *result = malloc(n);//第一级配置器直接使用malloc函数
			if (result == 0) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t)
		{
			free(p);//第一级配置器直接free
		}

		static  void* reallocate(void *p, size_t/*old_sz*/, size_t new_sz)
		{
			void *result = realloc(p, new_sz);//第一级配置器直接realloc
			if (result == 0)
				result = oom_realloc(p, new_sz);
			return result;
		}

		///以下函数可以指定自己的out of memory handler
		/*当我们看到set_malloc_handler有函数调用运算符和参数：
		(void (*f)())，说明这是个函数，以一个void (*)()类型的
		函数指针f做为参数(即f是一个函数指针，指向的函数为空参
		数列表，无返回值)，set_malloc_handler前面有一个*，所以
		这个函数应该返回一个指针，然后指针本身也有空参数列表()，
		因此该指针指向函数，该函数的返回值也是void，参数列表也
		为空*/
		static void(*set_malloc_handler(void(*f) ()))()
		{
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return old;
		}
	};
	//写那些静态成员
	void(*__malloc_alloc_template::__malloc_alloc_oom_handler)() = 0;

	void * __malloc_alloc_template::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;){//不断地尝试释放、配置、再释放、在配置

			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
			{
				__THROW_BAD_ALLOC;
			}
			(*my_malloc_handler)();//释放内存
			result = malloc(n);//配置内存
			if (result)return (result);
		}
	}
	void * __malloc_alloc_template::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;){//不断地尝试释放、配置、再释放、在配置

			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
			{
				__THROW_BAD_ALLOC;
			}
			(*my_malloc_handler)();//释放内存
			result = realloc(p, n);//配置内存
			if (result)return (result);
		}
	}
	typedef __malloc_alloc_template malloc_alloc;



	//第二级配置器
	enum { __ALIGN = 8 };
	enum { __MAX_BYTES = 128 };
	enum{ __NFREELISTS = __MAX_BYTES / __ALIGN };

	class __default_alloc_template{
	private:
		static size_t ROUND_UP(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1)& ~(__ALIGN - 1));
		}

		union obj
		{
			union obj * free_list_link;
			char client_data[1];
		};

		static obj* volatile free_list[__NFREELISTS];

		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}

		static void *refill(size_t n);

		static char* chunk_alloc(size_t size, int &nobjs);

		static char *start_free;

		static char *end_free;

		static size_t heap_size;

	public:
		static void* allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void* reallocate(void *p, size_t old_sz, size_t new_sz);
	};

	char *__default_alloc_template::start_free = 0;
	char *__default_alloc_template::end_free = 0;
	size_t __default_alloc_template::heap_size = 0;

	__default_alloc_template::obj * volatile __default_alloc_template::free_list[__NFREELISTS] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	void* __default_alloc_template::refill(size_t n)
	{
		int nobjs = 20;
		char * chunk = chunk_alloc(n, nobjs);
		obj * volatile * my_free_list;

		obj *result; int i;
		obj *current_obj, *next_obj;

		if (nobjs == 1)
			return (chunk);

		my_free_list = free_list + FREELIST_INDEX(n);

		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);

		for (i = 1;; i++)
		{
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i)
			{
				current_obj->free_list_link = 0;
				break;
			}
			else
			{
				current_obj->free_list_link = next_obj;
			}
		}
		return (result);
	}

	char* __default_alloc_template::chunk_alloc(size_t size, int & nobjs)
	{
		//size已经上调到8的倍数，nobjs是引用
		char *result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);

		}
		else
		{
			//内存池中剩余空间连一个区块的大小都无法提供 每次申请两倍的新内存+
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0)
			{
				//内存中还有一些零头
				//根据剩余的量找到适当的free_list
				obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			//配置heap空间用来补充内存池
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				//heap空间不足malloc（）失败
				int i;
				obj * volatile * my_free_list, *p;
				//检视手里面拥有的东西
				//对freelist进行搜索
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0)
					{
						//freelist中还有未用区块
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;

						//递归调用自己，为了修正nobjs
						return (chunk_alloc(size, nobjs));
						//融合残余零头终将被编入适当的freelist中备用
					}
				}

				end_free = 0;//山穷水尽没有任何内存可用
				
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);//使用第一级配置器尝试是否能将内存不足情况改变
			}

			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return (chunk_alloc(size, nobjs));
		}
	}

	void* __default_alloc_template::allocate(size_t n)
	{
		obj * volatile * my_free_list;
		obj * result;
		if (n > (size_t)__MAX_BYTES)
		{
			return (malloc_alloc::allocate(n));
		}
		my_free_list = free_list + FREELIST_INDEX(n);

		result = *my_free_list;
		if (result == 0){
			void *r = refill(ROUND_UP(n));
			return r;
		}
		*my_free_list = result->free_list_link;
		return (result);
	}

	void __default_alloc_template::deallocate(void *p, size_t n)
	{
		obj *q = (obj *)p;
		obj * volatile * my_free_list;
		if (n > (size_t)__MAX_BYTES)
		{
			return (malloc_alloc::deallocate(p,n));
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

	void *__default_alloc_template::reallocate(void *ptr, size_t old_sz, size_t new_sz)
	{
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}




	#ifdef __USE_MALLOC
		typedef __malloc_alloc_template malloc;
		typedef malloc_alloc alloc;
	#else
		typedef __default_alloc_template alloc;
	#endif



}

#endif
 