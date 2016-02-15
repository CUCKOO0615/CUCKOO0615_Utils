//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/02/16
//◇说明：指针工具集
//
// class PtrAutoGuard
// class PtrAutoGuard_Arr
//
//*************************************************

namespace CkPtrUtils
{
	/*
	** 离开作用域时,自动释放指针(非数组),并将指针赋值为NULL
	** 例:
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
	** 离开作用域时,自动释放指针(数组),并将指针赋值为NULL
	** 例:
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