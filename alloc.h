#ifndef _ALLOC_H_
#define _ALLOC_H_
#include<stdlib.h>
namespace zsco{
//��һ��������
//����汾�Ͳ���template���
	#if 0  
	#   include <new>  
	#   define __THROW_BAD_ALLOC throw bad_alloc  
	#elif !defined(__THROW_BAD_ALLOC)//�����ڴ����������  
	#   include <iostream>  
	#   define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)  
	#endif 


	class __malloc_alloc_template{
	private:
		//Ҫ�д����ڴ治��ĺ���
		static void *oom_malloc(size_t);
		static void *oom_realloc(void*, size_t);
		static void(*__malloc_alloc_oom_handler)();//����һ������ָ�룬��̬��Ա������ʼ��Ϊ0

	public:
		static void* allocate(size_t n)
		{
			void *result = malloc(n);//��һ��������ֱ��ʹ��malloc����
			if (result == 0) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t)
		{
			free(p);//��һ��������ֱ��free
		}

		static  void* reallocate(void *p, size_t/*old_sz*/, size_t new_sz)
		{
			void *result = realloc(p, new_sz);//��һ��������ֱ��realloc
			if (result == 0)
				result = oom_realloc(p, new_sz);
			return result;
		}

		///���º�������ָ���Լ���out of memory handler
		/*�����ǿ���set_malloc_handler�к�������������Ͳ�����
		(void (*f)())��˵�����Ǹ���������һ��void (*)()���͵�
		����ָ��f��Ϊ����(��f��һ������ָ�룬ָ��ĺ���Ϊ�ղ�
		���б��޷���ֵ)��set_malloc_handlerǰ����һ��*������
		�������Ӧ�÷���һ��ָ�룬Ȼ��ָ�뱾��Ҳ�пղ����б�()��
		��˸�ָ��ָ�������ú����ķ���ֵҲ��void�������б�Ҳ
		Ϊ��*/
		static void(*set_malloc_handler(void(*f) ()))()
		{
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return old;
		}
	};
	//д��Щ��̬��Ա
	void(*__malloc_alloc_template::__malloc_alloc_oom_handler)() = 0;

	void * __malloc_alloc_template::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;){//���ϵس����ͷš����á����ͷš�������

			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
			{
				__THROW_BAD_ALLOC;
			}
			(*my_malloc_handler)();//�ͷ��ڴ�
			result = malloc(n);//�����ڴ�
			if (result)return (result);
		}
	}
	void * __malloc_alloc_template::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;){//���ϵس����ͷš����á����ͷš�������

			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
			{
				__THROW_BAD_ALLOC;
			}
			(*my_malloc_handler)();//�ͷ��ڴ�
			result = realloc(p, n);//�����ڴ�
			if (result)return (result);
		}
	}
	typedef __malloc_alloc_template malloc_alloc;



	//�ڶ���������
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
		//size�Ѿ��ϵ���8�ı�����nobjs������
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
			//�ڴ����ʣ��ռ���һ������Ĵ�С���޷��ṩ ÿ���������������ڴ�+
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0)
			{
				//�ڴ��л���һЩ��ͷ
				//����ʣ������ҵ��ʵ���free_list
				obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			//����heap�ռ����������ڴ��
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				//heap�ռ䲻��malloc����ʧ��
				int i;
				obj * volatile * my_free_list, *p;
				//����������ӵ�еĶ���
				//��freelist��������
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0)
					{
						//freelist�л���δ������
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;

						//�ݹ�����Լ���Ϊ������nobjs
						return (chunk_alloc(size, nobjs));
						//�ںϲ�����ͷ�ս��������ʵ���freelist�б���
					}
				}

				end_free = 0;//ɽ��ˮ��û���κ��ڴ����
				
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);//ʹ�õ�һ�������������Ƿ��ܽ��ڴ治������ı�
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
 