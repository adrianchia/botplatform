#include "StdAfx.h"
#include "ManagerBase.h"


//////////////////////////////////////////////////////////////////////////
void RegisterAllJSonCmds( ManagerBase* man );


//////////////////////////////////////////////////////////////////////////
ManagerBase::ManagerBase() : m_tp(NULL), m_run(true), m_reset(true), m_checkth(NULL)
{
    RegisterAllJSonCmds(this);
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
        m_resetEvent.push_back( CreateEvent( NULL, FALSE, TRUE, NULL ) );
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

    // close event handles
    for ( size_t i = 0; i < m_ioevent.size(); ++i )
    {
        CloseHandle( m_ioevent[i] );
        CloseHandle( m_resetEvent[i] );
    }

    m_ioevent.clear();
    m_resetEvent.clear();
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

        m_reset = true;
        SetEvent( m_resetEvent[idx] );
    }
}

void ManagerBase::startRun()
{
    boost::lock_guard<boost::mutex> guard_(m_startRunMutex);

    // if any run thread need reset
    if ( m_reset )
    {
        // must wait all run complete
        if ( !m_resetEvent.empty() )
            WaitForMultipleObjects( m_resetEvent.size(), &m_resetEvent[0], TRUE, INFINITE );
        
        // then reset
        m_ioService.reset();

        // clear flag
        m_reset = false;

        // set all io-event signaled state, will run again
        for ( size_t i = 0; i < m_ioevent.size(); ++i )
            SetEvent( m_ioevent[i] );
    }
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
                
                bool timeout = item.token->isTimeOut();
                
                item.callback( timeout );

                if ( timeout )
                    item.token->reset();
            }
        }
    }
}

