#include "StdAfx.h"
#include "ClientBase.h"


CClientBase::CClientBase() : m_socket(NULL), m_failed(true)
{
    m_handleThis.reset( new HandleType::SafeHandleType(this) );
}

CClientBase::~CClientBase()
{
    m_handleThis->Clear();
    Close();
    delete m_socket;
}

bool CClientBase::Init( boost::asio::io_service& io_service )
{
    // initialize socket by io-service
    ATLASSERT( !m_socket );
    m_socket = new boost::asio::ip::tcp::socket( io_service );
    return true;
}

bool CClientBase::Connect( boost::asio::io_service& io_service, const std::string& host, int port )
{
    if ( !m_socket )
        return false;

    using namespace boost::asio::ip;

    try
    {
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, NumToStr(port));

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
    SetFailed();
    return false;
}

bool CClientBase::Send( const void* data, size_t dataLen )
{
    if ( !m_socket || !data || dataLen == 0 )
        return false;

    try
    {
        boost::asio::async_write( *m_socket, boost::asio::buffer(data, dataLen),
            boost::bind(&CClientBase::SendCallback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );

        return true;
    }
    catch (...)
    {
        SetFailed();
        return false;
    }
}

bool CClientBase::SyncSend( const void* data, size_t dataLen )
{
    if ( !m_socket || !data || dataLen == 0 )
        return false;

    try
    {
        boost::asio::write( *m_socket, boost::asio::buffer(data, dataLen) );
        return true;
    }
    catch (...)
    {
        SetFailed();
        return false;
    }
}

bool CClientBase::Recv( size_t dataLen )
{
    if ( !m_socket || dataLen == 0 )
        return false;

    if ( m_recvBuf.size() < dataLen )
        m_recvBuf.resize( dataLen );

    try
    {
        boost::asio::async_read( *m_socket, boost::asio::buffer(&m_recvBuf[0], dataLen),
            boost::bind(&CClientBase::RecvCallback, m_handleThis, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );

        return true;
    }
    catch ( ... )
    {
        SetFailed();
        return false;
    }
}

bool CClientBase::SyncRecv( size_t dataLen )
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
        SetFailed();
        return false;
    }
}

bool CClientBase::Close()
{
    if ( !m_socket )
        return false;

    ClearFailed();

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

bool CClientBase::OnSend( const boost::system::error_code& error, size_t bytes_transferred )
{
    return true;
}

bool CClientBase::OnRecv( const boost::system::error_code& error, size_t bytes_transferred )
{
    // if error happened, close
    if ( error )
    {
        Close();
        return false;
    }

    return true;
}

void CClientBase::SendCallback( CClientBase* p, const boost::system::error_code& error, size_t bytes_transferred )
{
}

void CClientBase::RecvCallback( HandleType handle, const boost::system::error_code& error, size_t bytes_transferred )
{
    SafeCallRecv( handle->GetPtr(), error, bytes_transferred );
}

void CClientBase::SafeCallRecv( CClientBase* p, const boost::system::error_code& error, size_t bytes_transferred )
{
    // check self valid and no exception will be throw
    BEGIN_SAFE

    if ( p )
        p->OnRecv( error, bytes_transferred );

    END_SAFE
}

