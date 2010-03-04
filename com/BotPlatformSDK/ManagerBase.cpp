#include "StdAfx.h"
#include "ManagerBase.h"



//////////////////////////////////////////////////////////////////////////

void CheckToken::reset()
{
    InterlockedExchange( &m_time, 0 );
}

void CheckToken::step()
{
    InterlockedIncrement( &m_time );
}

bool CheckToken::isTimeOut() const
{
    return m_time >= TIME_OUT;
}


//////////////////////////////////////////////////////////////////////////
ManagerBase::ManagerBase() : m_tp(NULL), m_run(true), m_checkth(NULL)
{
}

ManagerBase::~ManagerBase()
{
    close();
    clearCmds();
}

void ManagerBase::init( int threadCount )
{
    m_tp = new boost::threadpool::pool(threadCount);

    // create work threads according to count of cpu
    SYSTEM_INFO siSysInfo = {0};
    GetSystemInfo(&siSysInfo); 

    size_t cpus = siSysInfo.dwNumberOfProcessors;

    for ( size_t i = 0; i < cpus; ++i )
    {
        m_ioevent.push_back( CreateEvent( NULL, FALSE, FALSE, NULL ) );
        m_runth.push_back( new boost::thread( boost::bind(&ManagerBase::runIoService, this, i) ) );
    }

    m_checkth = new boost::thread( boost::bind(&ManagerBase::runCheckTime, this) );
}

void ManagerBase::close()
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
    startRun();

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

JSonCmdBase* ManagerBase::findJsonCmd( const std::string& name )
{
    JSonCmdMap::iterator it = m_cmdMap.find( name );
    if ( it != m_cmdMap.end() )
        return it->second;

    return NULL;
}

void ManagerBase::clearCmds()
{
    for ( JSonCmdMap::iterator it = m_cmdMap.begin(); it != m_cmdMap.end(); ++it )
    {
        JSonCmdBase* cmd = it->second;
        delete cmd;
    }

    m_cmdMap.clear();
}

void ManagerBase::runIoService( int idx )
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

void ManagerBase::startRun()
{
    for ( size_t i = 0; i < m_ioevent.size(); ++i )
        SetEvent( m_ioevent[i] );
}

void ManagerBase::runCheckTime()
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
                
                item.token->step();
                item.callback( item.token->isTimeOut() );
            }
        }
    }
}

