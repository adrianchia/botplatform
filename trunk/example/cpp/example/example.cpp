#include "stdafx.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlconv.h>
#include <string>
#include <iostream>

#import "../../../com/Bin/BotPlatformSDK.dll" named_guids no_namespace


//////////////////////////////////////////////////////////////////////////
#define BEGIN_(func)    std::string funcName = #func; try {
#define END_            } catch(const _com_error& e) { printLn( std::string("[ERROR:") + funcName + "] " + e.ErrorMessage() ); } return S_OK;
#define E_IF(hr)        if ( FAILED(hr) ) throw _com_error(hr);
#define PRINT_EVENT(x)  printLn( std::string("[EVENT:") + funcName + "] " + x );
#define PRINT_NULL(x)   BEGIN_(x) PRINT_EVENT(""); END_

#define MY_SINK_ENTRY_INFO(id, name) SINK_ENTRY_INFO(1, DIID__IRobotServerEvents, id, &RobotServerEventsImpl::name, &s_info_##name)

//////////////////////////////////////////////////////////////////////////
const std::string numToStr( int num )
{
    char buf[255] = {0};
    sprintf_s( buf, "%d", num );
    return buf;
}

const std::string hexToStr( int num, int padding )
{
    char buf[255] = {0};
    char fmt[]    = "%00X";

    fmt[2] += padding;
    sprintf_s( buf, fmt, num );
    return buf;
}

const std::string randStr()
{
    return numToStr( GetTickCount() % 100 );
}

void printLn( const std::string& info )
{
    std::cout << (info + "\n");
}

const std::string bstr2A( BSTR b )
{
    return (LPCSTR)CW2A(b);
}

const std::string operator+( const std::string& lhs, const _bstr_t& rhs )
{
    return lhs + (LPCSTR)rhs;
}

//////////////////////////////////////////////////////////////////////////
class CoInit
{
public:
    CoInit() { CoInitializeEx(NULL, COINIT_MULTITHREADED); }
    ~CoInit() { CoUninitialize(); }
};


//////////////////////////////////////////////////////////////////////////
_ATL_FUNC_INFO  s_info_onSessionOpened          = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onSessionClosed          = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onMessageReceived        = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onNudgeReceived          = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_onActivityAccepted       = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onActivityRejected       = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onActivityReceived       = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_BSTR } };
_ATL_FUNC_INFO  s_info_onActivityLoaded         = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onActivityClosed         = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_onUserJoined             = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onUserLeft               = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onUserAdd                = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_BSTR } };
_ATL_FUNC_INFO  s_info_onUserRemove             = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_BSTR } };

_ATL_FUNC_INFO  s_info_onExceptionCaught        = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_I4 } };

_ATL_FUNC_INFO  s_info_onFileAccepted           = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onFileRejected           = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onFileTransferEnded      = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onFileTransferCancelled  = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onFileTransferError      = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onFileInvited            = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_onWebcamAccepted         = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onWebcamRejected         = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onWebcamClosed           = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onWebcamError            = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_onUserUpdated            = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onPersonalMessageUpdated = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_BSTR } };

_ATL_FUNC_INFO  s_info_onContactListReceived    = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onInkReceived            = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_BSTR } };
_ATL_FUNC_INFO  s_info_onWinkReceived           = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onVoiceclipReceived      = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onTypingReceived         = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_onSceneUpdated           = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onDisplayPictureUpdated  = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_onColorSchemeUpdated     = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_I4 } };


static const char commandList[] = 
    "*****************************************\r"
    "**  BOTPLATFORM SDK DEMO COMMAND LIST  **\r"
    "*****************************************\r"
    " preface --- test message preface. \r"
    " emo ------- test emoticon.\r"
    " typing ---- test typing info.\r"
    " nudge ----- test nudge.\r"
    " ink ------- test ink.\r"
    " wink ------ test wink.\r"
    " voice ----- test voiceclip.\r"
    " file ------ test file transfer.\r"
    " p4 -------- test msn activity.\r"
    " name ------ set friendly name.\r"
    " psm ------- set personal message.\r"
    " dp -------- set display picture.\r"
    " ddp ------- set delux display picture.\r"
    " scene ----- set scene.\r"
    " color ----- set color scheme.\r"
    " invite ---- invite user.\r"
    " push ------ push message.\r"
    " create ---- create session.\r"
    " close ----- close session.\r"
    " help ------ print this command list.\r"
    " ? --------- print this command list.\r";

class RobotServerEventsImpl : 
    public IDispEventSimpleImpl<1, RobotServerEventsImpl, &DIID__IRobotServerEvents>
{
public:
    BEGIN_SINK_MAP(RobotServerEventsImpl)
        MY_SINK_ENTRY_INFO(1,  onSessionOpened)
        MY_SINK_ENTRY_INFO(2,  onSessionClosed)
        MY_SINK_ENTRY_INFO(3,  onMessageReceived)
        MY_SINK_ENTRY_INFO(4,  onNudgeReceived)

        MY_SINK_ENTRY_INFO(5,  onActivityAccepted)
        MY_SINK_ENTRY_INFO(6,  onActivityRejected)
        MY_SINK_ENTRY_INFO(7,  onActivityReceived)
        MY_SINK_ENTRY_INFO(8,  onActivityLoaded)
        MY_SINK_ENTRY_INFO(9,  onActivityClosed)

        MY_SINK_ENTRY_INFO(10, onUserJoined)
        MY_SINK_ENTRY_INFO(11, onUserLeft)
        MY_SINK_ENTRY_INFO(12, onUserAdd)
        MY_SINK_ENTRY_INFO(13, onUserRemove)

        MY_SINK_ENTRY_INFO(14, onExceptionCaught)

        MY_SINK_ENTRY_INFO(15, onFileAccepted)
        MY_SINK_ENTRY_INFO(16, onFileRejected)
        MY_SINK_ENTRY_INFO(17, onFileTransferEnded)
        MY_SINK_ENTRY_INFO(18, onFileTransferCancelled)
        MY_SINK_ENTRY_INFO(19, onFileTransferError)
        MY_SINK_ENTRY_INFO(20, onFileInvited)
        
        MY_SINK_ENTRY_INFO(21, onWebcamAccepted)
        MY_SINK_ENTRY_INFO(22, onWebcamRejected)
        MY_SINK_ENTRY_INFO(23, onWebcamClosed)
        MY_SINK_ENTRY_INFO(24, onWebcamError)

        MY_SINK_ENTRY_INFO(25, onUserUpdated)
        MY_SINK_ENTRY_INFO(26, onPersonalMessageUpdated)
 
        MY_SINK_ENTRY_INFO(27, onContactListReceived)
        MY_SINK_ENTRY_INFO(28, onInkReceived)
        MY_SINK_ENTRY_INFO(29, onWinkReceived)
        MY_SINK_ENTRY_INFO(30, onVoiceclipReceived)
        MY_SINK_ENTRY_INFO(31, onTypingReceived)

        MY_SINK_ENTRY_INFO(32, onSceneUpdated)
        MY_SINK_ENTRY_INFO(33, onDisplayPictureUpdated)
        MY_SINK_ENTRY_INFO(34, onColorSchemeUpdated)
    END_SINK_MAP()

private:
    HRESULT __stdcall onSessionOpened(IRobotSession* session)
    {
        BEGIN_(onSessionOpened)

        PRINT_EVENT( std::string("user=") + session->GetUsers()->Item(0)->ID + 
            ",friendly=" + session->GetUsers()->Item(0)->FriendlyName + 
            ",clientId=" + numToStr(session->GetUsers()->Item(0)->ClientID) +
            ",status="   + session->GetUsers()->Item(0)->Status );

        if ( OPEN_MODE_CONV_OPEN == session->GetOpenMode() )
        {
            session->SendText( commandList );
        }

        END_
    }

    HRESULT __stdcall onSessionClosed(IRobotSession* session)
    {
        PRINT_NULL(onSessionClosed);
    }

    HRESULT __stdcall onMessageReceived( IRobotSession* session, IRobotMessage* message )
    {
        BEGIN_(onMessageReceived)

        std::string cmd(bstr2A(message->Text));

        PRINT_EVENT( std::string("message=") + cmd );

        std::string param;
        size_t sp = cmd.find(':');

        if ( sp != std::string::npos )
        {
            param = cmd.substr(sp+1);
            cmd = cmd.substr(0, sp);
        }

        if ( cmd == "help" || cmd == "?" )
        {
            session->SendText( commandList );
        }
        else if ( cmd == "preface" ) 
        {
            IRobotMessagePtr msg = session->CreateMessage();
            msg->Signature = (std::string("preface-") + randStr()).c_str();
            msg->Text = "test change preface";
            session->SendIM( msg );
        }
        else if ( cmd == "emo" )
        {
            IRobotMessagePtr msg = session->CreateMessage();
            msg->RegisterEmoticon("(1)", "bear.png");
            msg->RegisterEmoticon("(2)", "beaver.png");
            msg->RegisterEmoticon("(3)", "balloon.png");
            msg->Text = "a(1)b(2)c(3)d";
            session->SendIM( msg );
        }
        else if ( cmd == "typing" )
        {
            session->SendTyping();
        }
        else if ( cmd == "nudge" )
        {
            session->SendNudge();
        }
        else if ( cmd == "wink" )
        {
            const char* k = "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";
            session->SendWink( "wink.data", k );
        }
        else if ( cmd == "ink" )
        {
            session->SendInk( "AEwcA4CABB0DugEKAwRIEUVkGRQyCACAFAIAACBCMwgAwAwCAAAgQhWrqtNBq6rTQQAAAD4AAFY+\r\nHgMCBDQKBgEJSYAHVgoGAQl2AAdY" );
        }
        else if ( cmd == "voice" )
        {
            session->SendVoiceclip( "voiceclip.wav" );
        }
        else if ( cmd == "file" )
        {
            session->SendFile( "file.txt", "" );
        }
        else if ( cmd == "p4" )
        {
            session->SendActivity( "http://botplatform.com","botplatform home" );
        }
        else if ( cmd == "name" )
        {
            if ( param.empty() )
                param = "name-" + randStr();

            m_server->SetDisplayName( session->Robot, param.c_str() );
        }
        else if ( cmd == "psm" )
        {
            if ( param.empty() )
                param = "psm-" + randStr();

            m_server->SetPersonalMessage( session->Robot, param.c_str() );
        }
        else if ( cmd == "dp" )
        {
            m_server->SetDisplayPicture( "", "dp.png" );
        }
        else if ( cmd == "ddp" )
        {
            m_server->SetDisplayPictureEx( "", "dp.png", "ddp.dat" );
        }
        else if ( cmd == "scene" )
        {
            m_server->SetScene( "", "scene.png" );
        }
        else if ( cmd == "color" )
        {
            m_server->SetColorScheme( "", 0xFF0000 );
        }
        else if ( cmd == "invite" )
        {
            if ( !param.empty() )
                session->InviteUser( param.c_str() );
        }
        else if ( cmd == "fl" )
        {
            m_server->RequestContactList( session->Robot );
        }
        else if ( cmd == "push" )
        {
            if ( !param.empty() )
                m_server->PushMessage( session->Robot, param.c_str(), "hello" );
        }
        else if ( cmd == "create" )
        {
            if ( !param.empty() )
                m_server->CreateSession( session->Robot, param.c_str() );
        }
        else if ( cmd =="close" )
        {
            session->Close();
        }
        else
        {
            std::string text = "fontname: " + message->FontName + "\r";
            text += "fontstyle: " + numToStr(message->FontStyle) + "\r";
            text += "fontcolor: 0x" + hexToStr(0xFFFFFF & message->FontColor, 6) + "\r";
            text += "message: " + message->Text + "\r";
            session->SendText( text.c_str() );
        }
        
        END_
    }

    HRESULT __stdcall onNudgeReceived(IRobotSession* session)
    {
        PRINT_NULL(onNudgeReceived);
    }

    HRESULT __stdcall onActivityAccepted(IRobotSession* session)
    {
        PRINT_NULL(onActivityAccepted);
    }

    HRESULT __stdcall onActivityRejected(IRobotSession* session)
    {
        PRINT_NULL(onActivityRejected);
    }

    HRESULT __stdcall onActivityReceived(IRobotSession* session, BSTR data)
    {
        BEGIN_(onActivityReceived)
        PRINT_EVENT(std::string("data=") + bstr2A(data));
        END_
    }

    HRESULT __stdcall onActivityLoaded(IRobotSession* session)
    {
        PRINT_NULL(onActivityLoaded);
    }

    HRESULT __stdcall onActivityClosed(IRobotSession* session)
    {
        PRINT_NULL(onActivityClosed);
    }

    HRESULT __stdcall onUserJoined(IRobotSession* session, IRobotUser* user)
    {
        PRINT_NULL(onUserJoined);
    }

    HRESULT __stdcall onUserLeft(IRobotSession* session, IRobotUser* user)
    {
        PRINT_NULL(onUserLeft);
    }

    HRESULT __stdcall onUserAdd(BSTR robot, BSTR user)
    {
        BEGIN_(onUserAdd)
        PRINT_EVENT(std::string("robot=") + bstr2A(robot) + ",user=" + bstr2A(user));
        END_
    }

    HRESULT __stdcall onUserRemove(BSTR robot, BSTR user)
    {
        BEGIN_(onUserRemove)
        PRINT_EVENT(std::string("robot=") + bstr2A(robot) + ",user=" + bstr2A(user));
        END_
    }

    HRESULT __stdcall onExceptionCaught(IRobotSession* session, LONG cause)
    {
        BEGIN_(onExceptionCaught)
        PRINT_EVENT( "cause=" + numToStr(cause) );
        END_
    }

    HRESULT __stdcall onFileAccepted(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(onFileAccepted)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + numToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall onFileRejected(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(onFileRejected)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + numToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall onFileTransferEnded(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(onFileTransferEnded)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + numToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall onFileTransferCancelled(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(onFileTransferCancelled)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + numToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall onFileTransferError(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(onFileTransferError)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + numToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall onFileInvited(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(onFileInvited)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + numToStr(fileDescriptor->Size) + ",thumbnail=" + fileDescriptor->Thumbnail );
        END_
    }

    HRESULT __stdcall onWebcamAccepted(IRobotSession* session)
    {
        PRINT_NULL(onWebcamAccepted);
    }

    HRESULT __stdcall onWebcamRejected(IRobotSession* session)
    {
        PRINT_NULL(onWebcamRejected);
    }

    HRESULT __stdcall onWebcamClosed(IRobotSession* session)
    {
        PRINT_NULL(onWebcamClosed);
    }

    HRESULT __stdcall onWebcamError(IRobotSession* session)
    {
        PRINT_NULL(onWebcamError);
    }

    HRESULT __stdcall onUserUpdated(BSTR robot, IRobotUser* user)
    {
        BEGIN_(onUserUpdated)
        PRINT_EVENT("robot=" + bstr2A(robot) + ",friendly=" + user->FriendlyName + ",status=" + user->Status);
        END_
    }

    HRESULT __stdcall onPersonalMessageUpdated(BSTR robot, BSTR user, BSTR personalMessage)
    {
        BEGIN_(onPersonalMessageUpdated)
        PRINT_EVENT("robot=" + bstr2A(robot) + ",user=" + bstr2A(user) + ",psm=" + bstr2A(personalMessage));
        END_
    }

    HRESULT __stdcall onContactListReceived(BSTR robot, IRobotUsers* contactList)
    {
        BEGIN_(onContactListReceived)

        size_t count = contactList->Count();
        PRINT_EVENT( "robot=" + bstr2A(robot) + ", size=" + numToStr(count) );
        
        for ( size_t i = 0; i < count; ++i )
        {
            IRobotUserPtr user = contactList->Item(i);
            std::cout << user->FriendlyName << std::endl;
        }

        END_
    }

    HRESULT __stdcall onInkReceived(IRobotSession* session, BSTR ink)
    {
        BEGIN_(onInkReceived)
        PRINT_EVENT( "size=" + numToStr(SysStringLen(ink)) );
        session->SendInk( ink );
        END_
    }

    HRESULT __stdcall onWinkReceived(IRobotSession* session, IRobotResource* resource)
    {
        BEGIN_(onWinkReceived)
        PRINT_EVENT( "name=" + resource->Name + ",size=" + numToStr(resource->Size) );
        END_
    }

    HRESULT __stdcall onVoiceclipReceived(IRobotSession* session, IRobotResource* resource)
    {
        BEGIN_(onVoiceclipReceived)
        PRINT_EVENT( "name=" + resource->Name + ",size=" + numToStr(resource->Size) );
        END_
    }

    HRESULT __stdcall onTypingReceived(IRobotSession* session)
    {
        PRINT_NULL(onTypingReceived);
    }

    HRESULT __stdcall onSceneUpdated(BSTR robot, BSTR user, IRobotResource* resource)
    {
        BEGIN_(onSceneUpdated)
        PRINT_EVENT( "robot=" + bstr2A(robot) + ",user=" + bstr2A(user) + ",resname=" + resource->Name );
        END_
    }

    HRESULT __stdcall onDisplayPictureUpdated(BSTR robot, BSTR user, IRobotResource* resource)
    {
        BEGIN_(onDisplayPictureUpdated)
        PRINT_EVENT( "robot=" + bstr2A(robot) + ",user=" + bstr2A(user) + ",resname=" + resource->Name );
        m_server->RequestResource( robot, user, resource, "http://www.xiaoi.com" );
        END_
    }

    HRESULT __stdcall onColorSchemeUpdated(BSTR robot, BSTR user, LONG colorScheme)
    {
        BEGIN_(onColorSchemeUpdated)
        PRINT_EVENT( "robot=" + bstr2A(robot) + ",user=" + bstr2A(user) + ",colorScheme=" + numToStr(colorScheme) );
        END_
    }

public:
    HRESULT setServer( IRobotServerPtr server )
    {
        m_server = server;
        return DispEventAdvise( server );
    }

private:
    IRobotServerPtr m_server;
};


//////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
    CoInit coinit_;
    BEGIN_(main)

    CComPtr<IRobotServerFactory> spRobotServerFactory;
    E_IF( spRobotServerFactory.CoCreateInstance( CLSID_RobotServerFactory, NULL, CLSCTX_INPROC ) );
    spRobotServerFactory->Init( 2 );

    IRobotServerPtr spRobotServer = spRobotServerFactory->CreateRobotServer( "bottest.com", 6602 );
    RobotServerEventsImpl eventImpl;
    E_IF( eventImpl.setServer( spRobotServer ) );
    spRobotServer->Login( "SP000125", "test111", 60000 );

    std::string cmd;
    while ( true )
    {
        std::cin >> cmd;
        if ( cmd == "exit" )
            break;
    }
    
    spRobotServer->Logout();
    spRobotServerFactory->Destroy();

    END_
	return 0;
}

