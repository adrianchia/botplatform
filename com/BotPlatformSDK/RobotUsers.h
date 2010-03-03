// RobotUsers.h : CRobotUsers ������

#pragma once
#include "resource.h"       // ������

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif


class CRobotUser;

// CRobotUsers

class ATL_NO_VTABLE CRobotUsers :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotUsers, &CLSID_RobotUsers>,
	public IRobotUsers
{
public:
    typedef boost::unordered_map<std::string, CRobotUser*> RobotUserMap;
    typedef std::vector<CRobotUser*> RobotUserVector;

public:
	CRobotUsers()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTUSERS)


BEGIN_COM_MAP(CRobotUsers)
	COM_INTERFACE_ENTRY(IRobotUsers)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease();

public:
    STDMETHOD(Item)(LONG idx, IRobotUser** ppUser);
    STDMETHOD(Count)(LONG* pVal);

public:
    void addUser( CRobotUser* user );
    void removeUser( const std::string& userId );
    
    CRobotUser* getUser( const std::string& userId );
    CRobotUser* getUser( size_t idx );

    size_t getUserCount();

private:
    RobotUserMap    m_userMap;
    RobotUserVector m_userVector;
    boost::mutex    m_userMutex;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotUsers), CRobotUsers)
