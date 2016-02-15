//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/02/16
//��˵����ָ�빤�߼�
//
// class PtrAutoGuard
// class PtrAutoGuard_Arr
//
//*************************************************

namespace CkPtrUtils
{
	/*
	** �뿪������ʱ,�Զ��ͷ�ָ��(������),����ָ�븳ֵΪNULL
	** ��:
	** Type* t = new Type;
	** PtrAutoGuard<Type> pag(&t);
	*/
	template<typename T>
	class PtrAutoGuard
		:m_ptr(NULL)
	{	
		T** m_ptr;
	public:
		PtrAutoGuard(T** ptr)   { m_ptr = ptr; };
		~PtrAutoGuard()         { delete (*m_ptr); (*m_ptr) = NULL};
	};

	/*
	** �뿪������ʱ,�Զ��ͷ�ָ��(����),����ָ�븳ֵΪNULL
	** ��:
	** Type* t = new Type[SIZE];
	** PtrAutoGuard_Arr<Type> paga(&t);
	*/
	template<typename T>
	class PtrAutoGuard_Arr
		:m_ptr(NULL)
	{
		T** m_ptr;
	public:
		PtrAutoGuard_Arr(T** ptr)   { m_ptr = ptr; };
		~PtrAutoGuard_Arr()         { delete [] (*m_ptr); (*m_ptr) = NULL };
	};

};