//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/02/16
//��˵����ָ�빤��
//*************************************************
#pragma once

#ifndef NULL
#define NULL 0
#endif 

//�ͷŶ���ָ��p, ����ֵΪNULL
#define CKSAFE_RELEASE_OBJ(pObj)    if(pObj){ delete pObj; pObj = NULL; }

//�ͷ�����ָ��p, ����ֵΪNULL
#define CKSAFE_RELEASE_ARR(pArr)    if(pArr){ delete [] pArr; pArr = NULL;}

namespace CkPtrUtils
{
    /*
    ** �뿪������ʱ,�Զ��ͷ�ָ��
    ** @Param nSize: ָ��ָ��������С
    ** --------------------------------------------------
    ** ��:
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
	** �뿪������ʱ,�Զ��ͷ�ָ��,����ָ�븳ֵΪNULL
	** @Param bIsArr: �Ƿ���ָ�������ָ��,�� true, �� false
    ** --------------------------------------------------
    ** ��:
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