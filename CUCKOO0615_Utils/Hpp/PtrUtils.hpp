//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/02/16
//◇说明：指针工具
//*************************************************
#pragma once

#ifndef NULL
#define NULL 0
#endif 

//释放对象指针p, 并赋值为NULL
#define CKSAFE_RELEASE_OBJ(pObj)    if(pObj){ delete pObj; pObj = NULL; }

//释放数组指针p, 并赋值为NULL
#define CKSAFE_RELEASE_ARR(pArr)    if(pArr){ delete [] pArr; pArr = NULL;}

namespace CkPtrUtils
{
    /*
    ** 离开作用域时,自动释放指针
    ** @Param nSize: 指针指向的数组大小
    ** --------------------------------------------------
    ** 例:
    **     CkPtrUtils::LocalPtr<Type> lp(10);
    ** --------------------------------------------------
    */
    template<typename T>
    class LocalPtr
    {
    public:
        LocalPtr(size_t nSize) :m_nSize(nSize), m_ptr(NULL)
        {
            if (!m_nSize)
                return;
            m_ptr = (1 == m_nSize) ? new T : new T[m_nSize];
        }

    public:
        operator T* ()                  { return m_ptr; }
        T* operator -> ()               { return m_ptr; }
        bool operator == (LocalPtr& lp) { return !::memcmp(m_ptr, (T*)lp); }
        bool operator != (LocalPtr& lp) { return ::memcmp(m_ptr, (T*)lp); }

        //////////////////////////////////////////////////////////////////////////
    private:
        int m_nSize;
        T* m_ptr;

        LocalPtr(LocalPtr& lp){}
        T* operator = (T* t) { return NULL };

    public:
        ~LocalPtr()
        {
            if (m_nSize)
                if (1 == m_nSize)
                    delete m_ptr;
                else
                    delete[] m_ptr;
        }
    };
    
	/*
	** 离开作用域时,自动释放指针,并将指针赋值为NULL
	** @Param bIsArr: 是否是指向数组的指针,是 true, 否 false
    ** --------------------------------------------------
    ** 例:
    **     Type* t = new Type;
    **     CkPtrUtils::PtrScopeGuard<Type> pag(&t, false);
    **     Type* t = new Type[N];
    **     CkPtrUtils::PtrScopeGuard<Type> pag(&t, true);
    ** --------------------------------------------------
	*/
	template<typename T>
	class PtrScopeGuard
	{
        T** m_ptr;
        bool m_bIsArr;
    public:
        PtrScopeGuard(T** ptr, bool bIsArr) :
            m_ptr(ptr),
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