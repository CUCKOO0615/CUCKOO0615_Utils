//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/02/16
//◇说明：指针工具集
//*************************************************
#pragma once

#ifndef NULL
#define NULL 0
#endif 

namespace CkPtrUtils
{
	/*
	** 离开作用域时,自动释放指针,并将指针赋值为NULL
	** 例: 
	** Type* t = new Type;
	** CkPtrUtils::PtrScopeGuard<Type> pag(&t, false);
	** Type* t = new Type[N];
	** CkPtrUtils::PtrScopeGuard<Type> pag(&t, true);
	** @Param bIsArr: 是否是指向数组的指针,是 true, 否 false
	*/
	template<typename T>
	class PtrScopeGuard
	{
		T** m_ptr;
		bool m_bIsArr;
	public:
		PtrScopeGuard(T** ptr, bool bIsArr)
			:m_ptr(ptr),
			m_bIsArr(bIsArr)
		{
		};
		~PtrScopeGuard()
		{
			if (m_bIsArr)
				delete[](*m_ptr);
			else
				delete (*m_ptr);
			(*m_ptr) = NULL;
		};
	};
};