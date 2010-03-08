#include "StdAfx.h"
#include "ClientBase.h"
#include "ManagerBase.h"


ClientBase::ClientBase() : m_serverMan(NULL), m_socket(NULL), m_failed(true)
{
    m_handleThis.reset( new HandleType::SafeHandleType(this) );
}

ClientBase::~ClientBase()
{
    m_handleThis->Clear();
    close();
    unRegisterCheckToken();
    delete m_socket;
}

bool ClientBase::init( ManagerBase* manBase )
{
    // initialize socket by io-service
    ATLASSERT( !m_socket && manBase );
    m_serverMan = manBase;
    m_socket = new boost::asio::ip::tcp::socket( manBase->getIOService() );
    return true;
}

bool ClientBase::connect( boost::asio::io_service& io_service, const std::string& host, int port )
{
    if ( !m_socket )
    {
        //m_socket = new boost::asio::ip::tcp::socket( manBase->getIOService() );
        return false;
    }

    using namespace boost::asio::ip;

    try
    {
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, numToStr(port));

        tcp::resolver::iterator beg = resolver.resolve(query);
        tcp::resolver::iterator end;

        // get all ip-list by host name
        std::vector<tcp::resolver::iterator> ipList;
        for ( tcp::resolver::iterator it = beg; it != end; ++it )
            ipList.push_back(it);

        // get a random ip from ip-list
        size_t ipfirst = rand() % ipList.size();

        boost::system::error_code error = boost::asio::error::host_not_found;

        for ( size_t i = 0; i < ipList.size(); ++i )
        {
            size_t pos = (ipfirst + i) % ipList.size();

            m_socket->close();
            m_socket->connect( *(ipList[pos]), error );

            // ok
            if ( !error )
                return true;
        }
    }
    catch (...)
    {
    }

    // failed connect
    setFailed();
    return false;
}

bool ClientBase::send( const void* data, size_t dataLen )
{
    if ( !m_socket || !data || dataLen == 0 )
        return false;

    try
    {
        boost::asio::async_write( *m_socket, boost::asio::buffer(data, dataLen),
            boost::bind(&ClientBase::sendCallback, m_handleThis, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );

        // if ok, rest check keep-alive time
        if ( m_checkToken )
            m_checkToken->reset();

        return true;
    }
    catch (...)
    {
        setFailed();
        return false;
    }
}

bool ClientBase::syncSend( const void* data, size_t dataLen )
{
    if ( !m_socket || !data || dataLen == 0 )
        return false;

    try
    {
        boost::asio::write( *m_socket, boost::asio::buffer(data, dataLen) );

        // if ok, rest check keep-alive time
        if ( m_checkToken )
            m_checkToken->reset();

        return true;
    }
    catch (...)
    {
        setFailed();
        return false;
    }
}

bool ClientBase::recv( size_t dataLen )
{
    if ( !m_socket || dataLen == 0 )
        return false;

    if ( m_recvBuf.size() < dataLen )
        m_recvBuf.resize( dataLen );

    try
    {
        boost::asio::async_read( *m_socket, boost::asio::buffer(&m_recvBuf[0], dataLen),
            boost::bind(&ClientBase::recvCallback, m_handleThis, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );

        return true;
    }
    catch ( ... )
    {
        setFailed();
        return false;
    }
}

bool ClientBase::syncRecv( size_t dataLen )
{
    if ( !m_socket || dataLen == 0 )
        return false;

    if ( m_recvBuf.size() < dataLen )
        m_recvBuf.resize( dataLen );

    try
    {
        boost::asio::read( *m_socket, boost::asio::buffer(&m_recvBuf[0], dataLen) );
        return true;
    }
    catch (...)
    {
        setFailed();
        return false;
    }
}

bool ClientBase::close()
{
    if ( !m_socket )
        return false;

    clearFailed();

    try
    {
        m_socket->shutdown( boost::asio::ip::tcp::socket::shutdown_both );
    }
    catch (...)
    {
    }

    try
    {
        m_socket->close();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void ClientBase::registerCheckToken()
{
    if ( !m_serverMan )
        return;

    if ( !m_checkToken )
        m_checkToken = m_serverMan->registerCheck( this, boost::bind(&ClientBase::checkNetworkCallBack, m_handleThis, _1) );
}

void ClientBase::unRegisterCheckToken()
{
    if ( !m_serverMan )
        return;

    if ( m_checkToken )
    {
        m_serverMan->unRegisterCheck(this);
    }
}

bool ClientBase::onSend( const boost::system::error_code& error, size_t bytes_transferred )
{
    // if error happened, close
    if ( error )
    {
        setFailed();
        return false;
    }

    return true;
}

bool ClientBase::onRecv( const boost::system::error_code& error, size_t bytes_transferred )
{
    // if error happened, close
    if ( error )
    {
        setFailed();
        return false;
    }

    return true;
}

void ClientBase::onCheckNetwork( bool needKeepAlive )
{
}

void ClientBase::sendCallback( HandleType handle, const boost::system::error_code& error, size_t bytes_transferred )
{
    safeCallSend( handle->GetPtr(), error, bytes_transferred );
}

void ClientBase::safeCallSend( ClientBase* p, const boost::system::error_code& error, size_t bytes_transferred )
{
    // check self valid and no exception will be throw
    BEGIN_SAFE

        if ( p )
            p->onSend( error, bytes_transferred );

    END_SAFE
}

void ClientBase::recvCallback( HandleType handle, const boost::system::error_code& error, size_t bytes_transferred )
{
    safeCallRecv( handle->GetPtr(), error, bytes_transferred );
}

void ClientBase::safeCallRecv( ClientBase* p, const boost::system::error_code& error, size_t bytes_transferred )
{
    // check self valid and no exception will be throw
    BEGIN_SAFE

    if ( p )
        p->onRecv( error, bytes_transferred );

    END_SAFE
}

void ClientBase::checkNetworkCallBack( HandleType handle, bool needKeepAlive )
{
    safeCallCheckNetwork( handle->GetPtr(), needKeepAlive );
}

void ClientBase::safeCallCheckNetwork( ClientBase* p, bool needKeepAlive )
{
    // check self valid and no exception will be throw
    BEGIN_SAFE

        if ( p )
            p->onCheckNetwork( needKeepAlive );

    END_SAFE
}

