//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/02/16
//��˵����ָ�빤�߼�
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
	** �뿪������ʱ,�Զ��ͷ�ָ��(������),����ָ�븳ֵΪNULL
	** ��:
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
	** �뿪������ʱ,�Զ��ͷ�ָ��(����),����ָ�븳ֵΪNULL
	** ��:
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