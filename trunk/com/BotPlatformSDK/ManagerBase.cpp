#include "StdAfx.h"
#include "ManagerBase.h"



//////////////////////////////////////////////////////////////////////////

void CheckToken::Reset()
{
    InterlockedExchange( &m_time, 0 );
}

void CheckToken::Step()
{
    InterlockedIncrement( &m_time );
}

bool CheckToken::IsTimeOut() const
{
    return m_time >= TIME_OUT;
}


//////////////////////////////////////////////////////////////////////////
CManagerBase::CManagerBase() : m_tp(0), m_run(true)
{
}

CManagerBase::~CManagerBase()
{
    Close();
    ClearCmds();
}

void CManagerBase::Init( int threadCount )
{
    m_tp = new boost::threadpool::pool(threadCount);

    // create work threads according to count of cpu
    SYSTEM_INFO siSysInfo = {0};
    GetSystemInfo(&siSysInfo); 

    size_t cpus = siSysInfo.dwNumberOfProcessors;

    for ( size_t i = 0; i < cpus; ++i )
    {
        m_ioevent.push_back( CreateEvent( NULL, FALSE, FALSE, NULL ) );
        m_runth.push_back( new boost::thread( boost::bind(&CManagerBase::RunIoService, this, i) ) );
    }

    m_checkth = new boost::thread( boost::bind(&CManagerBase::RunCheckTime, this) );
}

void CManagerBase::Close()
{
    // thread loop break
    m_run = false;

    // close thread pool
    if ( m_tp )
    {
        m_tp->wait();
        delete m_tp;
        m_tp = NULL;
    }

    // close io-service
    m_ioService.stop();

    // ensure io event no block
    StartRun();

    // close io-service threads
    for ( size_t i = 0; i < m_runth.size(); ++i )
    {
        m_runth[i]->join();
        delete m_runth[i];
    }
    m_runth.clear();

    // close check thread
    if ( m_checkth )
    {
        m_checkth->join();
        delete m_checkth;
        m_checkth = NULL;
    }

    // close io event handles
    for ( size_t i = 0; i < m_ioevent.size(); ++i )
        CloseHandle( m_ioevent[i] );

    m_ioevent.clear();
}

JSonCmdBase* CManagerBase::FindJsonCmd( const std::string& name )
{
    JSonCmdMap::iterator it = m_cmdMap.find( name );
    if ( it != m_cmdMap.end() )
        return it->second;

    return NULL;
}

void CManagerBase::ClearCmds()
{
    for ( JSonCmdMap::iterator it = m_cmdMap.begin(); it != m_cmdMap.end(); ++it )
    {
        JSonCmdBase* cmd = it->second;
        delete cmd;
    }

    m_cmdMap.clear();
}

void CManagerBase::RunIoService( int idx )
{
    while ( m_run )
    {
        WaitForSingleObject( m_ioevent[idx], INFINITE );

        if ( !m_run )
            return;

        try
        {
            m_ioService.run();
        }
        catch ( ... )
        {

        }
    }
}

void CManagerBase::StartRun()
{
    for ( size_t i = 0; i < m_ioevent.size(); ++i )
        SetEvent( m_ioevent[i] );
}

void CManagerBase::RunCheckTime()
{
    while ( m_run )
    {
        Sleep( CheckToken::TIME_INTER );

        if ( !m_run )
            return;

        {
            boost::lock_guard<boost::mutex> guard_(m_checkItemMutex);

            for ( CheckItemMap::iterator it = m_checkItemMap.begin(); it != m_checkItemMap.end(); ++it )
            {
                CheckItem& item = it->second;
                
                item.token->Step();
                item.callback( item.token->IsTimeOut() );
            }
        }
    }
}

