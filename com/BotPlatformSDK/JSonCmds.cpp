#include "StdAfx.h"
#include "JSonCmds.h"
#include "RobotServer.h"
#include "RobotSession.h"
#include "RobotUsers.h"
#include "RobotUser.h"
#include "RobotResource.h"
#include "RobotMessage.h"
//#include "RobotTransInvitation.h"
#include "RobotFileDescriptor.h"
#include "Misc.h"


// user's properties updated
class JSonCmdUserUpdated : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("userupdated")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CComPtr<IRobotUser> user;
        CRobotUser* realUser = NULL;

        HRESULT hr = CreateRealObject( &user, &realUser );
        if ( FAILED(hr) )
            return;

        realUser->setAll( root["body"] );
        pServer->Fire_UserUpdated( UTF8_2_BSTR(robotId), user );
    }
};

// user's personal message updated
class JSonCmdPsmUpdated : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("psmupdated")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        std::string psm = root["body"].asString();      
        pServer->Fire_PersonalMessageUpdated( UTF8_2_BSTR(robotId), UTF8_2_BSTR(userId), UTF8_2_BSTR(psm) );
    }
};

// user's display picture updated
class JSonCmdDpUpdated : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("dpupdated")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CComPtr<IRobotResource> res;
        CRobotResource* realRes = NULL;

        HRESULT hr = CreateRealObject( &res, &realRes );
        if ( FAILED(hr) )
            return;

        realRes->setAll( root["body"] );
        pServer->Fire_DisplayPictureUpdated( UTF8_2_BSTR(robotId), UTF8_2_BSTR(userId), res );
    }
};

// user's scene updated
class JSonCmdSceneUpdated : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("sceneupdated")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CComPtr<IRobotResource> res;
        CRobotResource* realRes = NULL;

        HRESULT hr = CreateRealObject( &res, &realRes );
        if ( FAILED(hr) )
            return;

        realRes->setAll( root["body"] );
        pServer->Fire_SceneUpdated( UTF8_2_BSTR(robotId), UTF8_2_BSTR(userId), res );
    }
};

// user's theme color updated
class JSonCmdColorUpdated : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("colorupdated")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        int colorScheme = root["body"].asInt();

        pServer->Fire_ColorSchemeUpdated( UTF8_2_BSTR(robotId), UTF8_2_BSTR(userId), colorScheme );
    }
};

// user added a robot
class JSonCmdUserAdded : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("useradded")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        pServer->Fire_UserAdd( UTF8_2_BSTR(robotId), UTF8_2_BSTR(userId) );
    }
};

// user removed a robot
class JSonCmdUserRemoved : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("userremoved")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        pServer->Fire_UserRemove( UTF8_2_BSTR(robotId), UTF8_2_BSTR(userId) );
    }
};

// conversation opened
class JSonCmdSessionOpened : public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("sessionopened")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        IRobotUser* user = NULL;
        CRobotUser* realUser = NULL;
        HRESULT hr = CreateRealObject( &user, &realUser );
        if ( FAILED(hr) )
            return;

        IRobotSession* session = NULL;
        CRobotSession* realSession = NULL;
        hr = CreateRealObject( &session, &realSession );
        if ( FAILED(hr) )
            return;

        int mode = root["body"]["mode"].asInt();

        realUser->setAll( root["body"]["user"] );
        ATLASSERT( userId == realUser->getID() );

        realSession->init( pServer, robotId, userId, sessionId, mode );
        realSession->getUsers()->addUser( realUser );

        pServer->AddSession( sessionId, realSession );
        pServer->Fire_SessionOpened( session );
    }
};

// conversation closed
class JSonCmdSessionClosed: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("sessionclosed")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CRobotSession* session = pServer->GetSession( sessionId );
        pServer->Fire_SessionClosed( session );
        pServer->RemoveSession( sessionId );
    }
};

// receive a message from one user
class JSonCmdMsg: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("msg")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        IRobotMessage* msg = NULL;
        CRobotMessage* realMsg = NULL;
        HRESULT hr = CreateRealObject( &msg, &realMsg );
        if ( FAILED(hr) )
            return;

        realMsg->setAll( root["body"] );
        CRobotSession* session = pServer->GetSession( sessionId );
        pServer->Fire_MessageReceived( session, msg );
    }
};

// user joined in conversation
class JSonCmdJoin: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("join")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        IRobotUser* user = NULL;
        CRobotUser* realUser = NULL;
        HRESULT hr = CreateRealObject( &user, &realUser );
        if ( FAILED(hr) )
            return;

        realUser->setAll( root["body"] );
        ATLASSERT( userId == realUser->getID() );

        CRobotSession* session = pServer->GetSession( sessionId );
        session->getUsers()->addUser( realUser );
        pServer->Fire_UserJoined( session, user );
    }
};

// user left conversation
class JSonCmdPart: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("part")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CRobotSession* session = pServer->GetSession( sessionId );
        CRobotUser*    user    = session->getUsers()->getUser( userId );

        pServer->Fire_UserLeft( session, user );
        session->getUsers()->removeUser( userId );
    }
};

// receive a nudge from one user
class JSonCmdNudge: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("nudge")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CRobotSession* session = pServer->GetSession( sessionId );
        pServer->Fire_NudgeReceived( session );
    }
};

// receive typing from one user
class JSonCmdTyping: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("typing")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CRobotSession* session = pServer->GetSession( sessionId );
        pServer->Fire_TypingReceived( session );
    }
};

// receive ink from one user
class JSonCmdInkmsg: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("inkmsg")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CRobotSession* session = pServer->GetSession( sessionId );
        std::string inkData = root["body"].asString();
        pServer->Fire_InkReceived(session, UTF8_2_BSTR(inkData) );
    }
};

// receive wink from one user
class JSonCmdWinkevent: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("winkevent")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CComPtr<IRobotResource> res;
        CRobotResource* realRes = NULL;

        HRESULT hr = CreateRealObject( &res, &realRes );
        if ( FAILED(hr) )
            return;

        realRes->setAll( root["body"] );
        CRobotSession* session = pServer->GetSession( sessionId );
        pServer->Fire_WinkReceived( session, res );
    }
};

// receive voice clip from one user
class JSonCmdVoliceclipevent: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("voliceclipevent")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CComPtr<IRobotResource> res;
        CRobotResource* realRes = NULL;

        HRESULT hr = CreateRealObject( &res, &realRes );
        if ( FAILED(hr) )
            return;

        realRes->setAll( root["body"] );
        CRobotSession* session = pServer->GetSession( sessionId );
        pServer->Fire_VoiceclipReceived( session, res );
    }
};

// receive activity message from one user
class JSonCmdAppmsg: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("appmsg")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        std::string data = root["data"].asString();
        CRobotSession* session = pServer->GetSession( sessionId );

        pServer->Fire_ActivityReceived( session, UTF8_2_BSTR(data) );
    }
};

// activity accepted/rejected/ready/close from one user
class JSonCmdAppevent: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("appevent")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        std::string repl = root["body"].asString();

        CRobotSession* session = pServer->GetSession( sessionId );

        if ( repl == "accept" )
        {
            session->setActive( true );
            pServer->Fire_ActivityAccepted( session );
        }
        else if ( repl == "reject" )
        {
            pServer->Fire_ActivityRejected( session );
        }
        else if ( repl == "ready" )
        {
            pServer->Fire_ActivityLoaded( session );
        }
        else if ( repl == "close" )
        {
            session->setActive( false );
            pServer->Fire_ActivityClosed( session );
        }
    }
};

// file transport invite/accepted/rejected/canceled/error from one user
class JSonCmdFileevent: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("fileevent")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CComPtr<IRobotFileDescriptor> desc;
        CRobotFileDescriptor* realDesc = NULL;

        HRESULT hr = CreateRealObject( &desc, &realDesc );
        if ( FAILED(hr) )
            return;

        std::string    eve      = root["body"]["event"].asString();
        CRobotSession* session  = pServer->GetSession( sessionId );

        realDesc->setAll( root["body"] );

        if ( eve == "invite" )
            pServer->Fire_FileInvited( session, desc );
        if ( eve == "accept" )
            pServer->Fire_FileAccepted( session, desc );
        else if ( eve == "reject" )
            pServer->Fire_FileRejected( session, desc );
        else if ( eve == "close" )
            pServer->Fire_FileTransferEnded( session, desc );
        else if ( eve == "cancel" )
            pServer->Fire_FileTransferCancelled( session, desc );
        else if ( eve == "error" )
            pServer->Fire_FileTransferError( session, desc );
    }
};

// webcam accepted/rejected/close/error from one user
class JSonCmdWebcamevent: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("webcamevent")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        std::string repl = root["body"].asString();

        CRobotSession* session = pServer->GetSession( sessionId );

        if ( repl == "accept" )
            pServer->Fire_WebcamAccepted(session);
        else if ( repl == "reject" )
            pServer->Fire_WebcamRejected(session);
        else if ( repl == "close" )
            pServer->Fire_WebcamClosed(session);
        else if ( repl == "error" )
            pServer->Fire_WebcamError( session );
    }
};

// receive userlist
class JSonCmdUserlist: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("userlist")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        CComPtr<IRobotUsers> userList;
        CRobotUsers* realUserList = NULL;

        HRESULT hr = CreateRealObject( &userList, &realUserList );
        if ( FAILED(hr) )
            return;

        for ( Json::Value::iterator it = root["body"].begin(), itend = root["body"].end(); it != itend; ++it )
        {
            IRobotUser*  user       = NULL;
            CRobotUser*  realUser   = NULL;

            HRESULT hr = CreateRealObject( &user, &realUser );
            if ( FAILED(hr) )
                return;

            realUser->setAll( *it );
            realUserList->addUser( realUser );
        }

        CRobotSession* session = pServer->GetSession( sessionId ); 
        pServer->Fire_ContactListReceived( UTF8_2_BSTR(robotId), userList );
    }
};

class JSonCmdError: public JSonCmdServerBase
{
    JSON_CMD_TYPE_NAME("error")

public:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root )
    {
        int         code = root["body"]["code"].asInt();
        std::string msg  = root["body"]["message"].asString();

        CRobotSession* session = pServer->GetSession( sessionId ); 
        pServer->Fire_ExceptionCaught( session, code );
    }
};

//////////////////////////////////////////////////////////////////////////
void RegisterAllJSonCmds( CManagerBase* man )
{
    #define REGISTER_CMD(type)  man->RegisterJSonCmd<type>();


    REGISTER_CMD(JSonCmdUserUpdated);
    REGISTER_CMD(JSonCmdPsmUpdated);
    REGISTER_CMD(JSonCmdDpUpdated);
    REGISTER_CMD(JSonCmdSceneUpdated);
    REGISTER_CMD(JSonCmdColorUpdated);
    REGISTER_CMD(JSonCmdUserAdded);
    REGISTER_CMD(JSonCmdUserRemoved);
    REGISTER_CMD(JSonCmdSessionOpened);
    REGISTER_CMD(JSonCmdSessionClosed);
    REGISTER_CMD(JSonCmdMsg);
    REGISTER_CMD(JSonCmdJoin);
    REGISTER_CMD(JSonCmdPart);
    REGISTER_CMD(JSonCmdNudge);
    REGISTER_CMD(JSonCmdTyping);
    REGISTER_CMD(JSonCmdInkmsg);
    REGISTER_CMD(JSonCmdWinkevent);
    REGISTER_CMD(JSonCmdVoliceclipevent);
    REGISTER_CMD(JSonCmdAppmsg);
    REGISTER_CMD(JSonCmdAppevent);
    REGISTER_CMD(JSonCmdFileevent);
    REGISTER_CMD(JSonCmdWebcamevent);
    REGISTER_CMD(JSonCmdUserlist);
    REGISTER_CMD(JSonCmdError);
}

