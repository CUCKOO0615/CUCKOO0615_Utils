//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/02/16
//◇说明：指针工具集
//
// class PtrAutoGuard
// class PtrAutoGuard_Arr
//
//*************************************************
#pragma once

#ifndef NULL
#define NULL 0
#endif 

namespace CkPtrUtils
{
	/*
	** 离开作用域时,自动释放指针(非数组),并将指针赋值为NULL
	** 例:
	** Type* t = new Type;
	** CkPtrUtils::PtrAutoGuard<Type> pag(&t);
	*/
	template<typename T>
	class PtrAutoGuard
	{
		T** m_ptr;
	public:
		PtrAutoGuard(T** ptr)
			:m_ptr(NULL) {
			m_ptr = ptr;
		};
		~PtrAutoGuard()  { delete (*m_ptr); (*m_ptr) = NULL; };
	};

	/*
	** 离开作用域时,自动释放指针(数组),并将指针赋值为NULL
	** 例:
	** Type* t = new Type[SIZE];
	** CkPtrUtils::PtrAutoGuard_Arr<Type> paga(&t);
	*/
	template<typename T>
	class PtrAutoGuard_Arr
	{
		T** m_ptr;
	public:
		PtrAutoGuard_Arr(T** ptr)
			:m_ptr(NULL)    {
			m_ptr = ptr;
		};
		~PtrAutoGuard_Arr() { delete[](*m_ptr); (*m_ptr) = NULL; };
	};

};