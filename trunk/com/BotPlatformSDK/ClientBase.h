#pragma once
#include "Misc.h"

class CClientBase
{
private:
    typedef CSafeHandlePtr<CClientBase> HandleType;

protected:
    CClientBase();

    virtual ~CClientBase();

protected:
    bool Init( boost::asio::io_service& io_service );

    bool Connect( boost::asio::io_service& io_service, const std::string& ip, int port );

    bool Send( const void* data, size_t dataLen );

    bool SyncSend( const void* data, size_t dataLen );

    bool Recv( size_t dataLen );

    bool SyncRecv( size_t dataLen );

    bool Close();

    void SetFailed()   { m_failed = true; }

    void ClearFailed() { m_failed = false; }

    bool IsFailed() const { return m_failed; }

protected:
    virtual bool OnSend( const boost::system::error_code& error, size_t bytes_transferred );

    virtual bool OnRecv( const boost::system::error_code& error, size_t bytes_transferred );

private:
    static void SendCallback( CClientBase* p, const boost::system::error_code& error, size_t bytes_transferred );

    static void RecvCallback( HandleType handle, const boost::system::error_code& error, size_t bytes_transferred );

    static void SafeCallRecv( CClientBase* p, const boost::system::error_code& error, size_t bytes_transferred );

protected:
    boost::asio::ip::tcp::socket*  m_socket;

    HandleType                     m_handleThis;

    std::string                    m_recvBuf;

    bool                           m_failed;
};
