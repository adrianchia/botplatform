#pragma once
#include "Misc.h"
#include "MiscObjBase.h"


class ManagerBase;

class ClientBase
{
private:
    typedef SafeHandlePtr<ClientBase> HandleType;

protected:
    ClientBase();

    virtual ~ClientBase();

protected:
    bool init( ManagerBase* manBase );

    bool connect( boost::asio::io_service& io_service, const std::string& host, int port );

    bool send( const void* data, size_t dataLen );

    bool syncSend( const void* data, size_t dataLen );

    bool recv( size_t dataLen );

    bool syncRecv( size_t dataLen );

    bool close();

    void setFailed()   { m_failed = true; }

    void clearFailed() { m_failed = false; }

    bool isFailed() const { return m_failed; }

    void registerCheckToken();

    void unRegisterCheckToken();

protected:
    virtual bool onSend( const boost::system::error_code& error, size_t bytes_transferred );

    virtual bool onRecv( const boost::system::error_code& error, size_t bytes_transferred );

    virtual void onCheckNetwork( bool needKeepAlive );

private:
    static void sendCallback( HandleType handle, const boost::system::error_code& error, size_t bytes_transferred );

    static void safeCallSend( ClientBase* p, const boost::system::error_code& error, size_t bytes_transferred );

    static void recvCallback( HandleType handle, const boost::system::error_code& error, size_t bytes_transferred );

    static void safeCallRecv( ClientBase* p, const boost::system::error_code& error, size_t bytes_transferred );

    static void checkNetworkCallBack( HandleType handle, bool needKeepAlive );

    static void safeCallCheckNetwork( ClientBase* p, bool needKeepAlive );

protected:
    ManagerBase*                   m_serverMan;

    boost::asio::ip::tcp::socket*  m_socket;

    HandleType                     m_handleThis;

    std::string                    m_recvBuf;

    bool                           m_failed;

    CheckTokenPtr                  m_checkToken;
};

