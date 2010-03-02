// TestCpp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlconv.h>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

#import "../Bin/BotPlatformSDK.dll" named_guids no_namespace /*raw_interfaces_only*/


//////////////////////////////////////////////////////////////////////////
#define BEGIN_(func)    std::string funcName = #func; try {
#define END_            } catch(const _com_error& e) { PrintLn( std::string("[ERROR:") + funcName + "] " + e.ErrorMessage() ); } return S_OK;
#define E_IF(hr)        if ( FAILED(hr) ) throw _com_error(hr);
#define PRINT_EVENT(x)  PrintLn( std::string("[EVENT:") + funcName + "] " + x );
#define PRINT_NULL(x)   BEGIN_(x) PRINT_EVENT(""); END_

#define MY_SINK_ENTRY_INFO(id, name) SINK_ENTRY_INFO(1, DIID__IRobotServerEvents, id, &RobotServerEventsImpl::name, &s_info_##name)

//////////////////////////////////////////////////////////////////////////
const std::string NumToStr( int num )
{
    char buf[255] = {0};
    sprintf_s( buf, "%d", num );
    return buf;
}

const std::string HexToStr( int num, int padding )
{
    char buf[255] = {0};
    char fmt[]    = "%00X";

    fmt[2] += padding;
    sprintf_s( buf, fmt, num );
    return buf;
}

const std::string RandStr()
{
    return NumToStr( GetTickCount() % 100 );
}

void PrintLn( const std::string& info )
{
    std::cout << (info + "\n");
}

const std::string BSTR2A( BSTR b )
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
    CoInit() { CoInitialize(NULL); }
    ~CoInit() { CoUninitialize(); }
};


//////////////////////////////////////////////////////////////////////////
// 事件测试
_ATL_FUNC_INFO  s_info_OnSessionOpened          = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnSessionClosed          = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnMessageReceived        = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnNudgeReceived          = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_OnActivityAccepted       = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnActivityRejected       = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnActivityReceived       = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_BSTR } };
_ATL_FUNC_INFO  s_info_OnActivityLoaded         = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnActivityClosed         = { CC_STDCALL, VT_EMPTY, 1, {  VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_OnUserJoined             = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnUserLeft               = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnUserAdd                = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_BSTR } };
_ATL_FUNC_INFO  s_info_OnUserRemove             = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_BSTR } };

_ATL_FUNC_INFO  s_info_OnExceptionCaught        = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_I4 } };

_ATL_FUNC_INFO  s_info_OnFileAccepted           = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnFileRejected           = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnFileTransferEnded      = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnFileTransferCancelled  = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnFileTransferError      = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnFileInvited            = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_OnWebcamAccepted         = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnWebcamRejected         = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnWebcamClosed           = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnWebcamError            = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };

_ATL_FUNC_INFO  s_info_OnUserUpdated            = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnPersonalMessageUpdated = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_BSTR } };

_ATL_FUNC_INFO  s_info_OnContactListReceived    = { CC_STDCALL, VT_EMPTY, 2, { VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnInkReceived            = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_BSTR } };
_ATL_FUNC_INFO  s_info_OnWinkReceived           = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnVoiceclipReceived      = { CC_STDCALL, VT_EMPTY, 2, { VT_UNKNOWN, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnTypingReceived         = { CC_STDCALL, VT_EMPTY, 1, { VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnSceneUpdated           = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnDisplayPictureUpdated  = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_UNKNOWN } };
_ATL_FUNC_INFO  s_info_OnColorSchemeUpdated     = { CC_STDCALL, VT_EMPTY, 3, { VT_BSTR, VT_BSTR, VT_I4 } };


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
    " webcam ---- test webcam.\r"
    " name ------ set friendly name.\r"
    " psm ------- set personal message.\r"
    " dp -------- set display picture.\r"
    " ddp ------- set delux display picture.\r"
    " scene ----- set scene.\r"
    " color ----- set color scheme.\r"
    " invite ---- invite user.\r"
    " push ------ push message.\r"
    " help ------ print this command list.\r"
    " ? --------- print this command list.\r";

class RobotServerEventsImpl : 
    public IDispEventSimpleImpl<1, RobotServerEventsImpl, &DIID__IRobotServerEvents>
{
public:
    BEGIN_SINK_MAP(RobotServerEventsImpl)
        MY_SINK_ENTRY_INFO(1,  OnSessionOpened)
        MY_SINK_ENTRY_INFO(2,  OnSessionClosed)
        MY_SINK_ENTRY_INFO(3,  OnMessageReceived)
        MY_SINK_ENTRY_INFO(4,  OnNudgeReceived)

        MY_SINK_ENTRY_INFO(5, OnActivityAccepted)
        MY_SINK_ENTRY_INFO(6, OnActivityRejected)
        MY_SINK_ENTRY_INFO(7, OnActivityReceived)
        MY_SINK_ENTRY_INFO(8, OnActivityLoaded)
        MY_SINK_ENTRY_INFO(9, OnActivityClosed)

        MY_SINK_ENTRY_INFO(10, OnUserJoined)
        MY_SINK_ENTRY_INFO(11, OnUserLeft)
        MY_SINK_ENTRY_INFO(12, OnUserAdd)
        MY_SINK_ENTRY_INFO(13, OnUserRemove)

        MY_SINK_ENTRY_INFO(14, OnExceptionCaught)

        MY_SINK_ENTRY_INFO(15, OnFileAccepted)
        MY_SINK_ENTRY_INFO(16, OnFileRejected)
        MY_SINK_ENTRY_INFO(17, OnFileTransferEnded)
        MY_SINK_ENTRY_INFO(18, OnFileTransferCancelled)
        MY_SINK_ENTRY_INFO(19, OnFileTransferError)
        MY_SINK_ENTRY_INFO(20, OnFileInvited)
        
        MY_SINK_ENTRY_INFO(21, OnWebcamAccepted)
        MY_SINK_ENTRY_INFO(22, OnWebcamRejected)
        MY_SINK_ENTRY_INFO(23, OnWebcamClosed)
        MY_SINK_ENTRY_INFO(24, OnWebcamError)

        MY_SINK_ENTRY_INFO(25, OnUserUpdated)
        MY_SINK_ENTRY_INFO(26, OnPersonalMessageUpdated)
 
        MY_SINK_ENTRY_INFO(27, OnContactListReceived)
        MY_SINK_ENTRY_INFO(28, OnInkReceived)
        MY_SINK_ENTRY_INFO(29, OnWinkReceived)
        MY_SINK_ENTRY_INFO(30, OnVoiceclipReceived)
        MY_SINK_ENTRY_INFO(31, OnTypingReceived)

        MY_SINK_ENTRY_INFO(32, OnSceneUpdated)
        MY_SINK_ENTRY_INFO(33, OnDisplayPictureUpdated)
        MY_SINK_ENTRY_INFO(34, OnColorSchemeUpdated)
    END_SINK_MAP()

private:
    HRESULT __stdcall OnSessionOpened(IRobotSession* session)
    {
        BEGIN_(OnSessionOpened)

        PRINT_EVENT( std::string("user=") + session->GetUsers()->Item(0)->ID + 
            ",friendly=" + session->GetUsers()->Item(0)->friendlyName + 
            ",clientId=" + NumToStr(session->GetUsers()->Item(0)->ClientID) +
            ",status="   + session->GetUsers()->Item(0)->Status );

        if ( OPEN_MODE_CONV_OPEN == session->GetOpenMode() )
        {
            session->SendText( commandList );
        }

        END_
    }

    HRESULT __stdcall OnSessionClosed(IRobotSession* session)
    {
        PRINT_NULL(OnSessionClosed);
    }

    HRESULT __stdcall OnMessageReceived( IRobotSession* session, IRobotMessage* message )
    {
        BEGIN_(OnMessageReceived)

        std::string cmd(BSTR2A(message->Text));

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
            msg->Signature = (std::string("preface-") + RandStr()).c_str();
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
            const char* k = "MIIIngYJKoZIhvcNAQcCoIIIjzCCCIsCAQExCzAJBgUrDgMCGgUAMCwGCSqGSIb3DQEHAaAfBB1SZ016K2JpeU1RSkxEeGxIWFVoZ0FOdFhpZDg9YaCCBrUwggaxMIIFmaADAgECAgoJlhkGAAEAAADYMA0GCSqGSIb3DQEBBQUAMHwxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpXYXNoaW5ndG9uMRAwDgYDVQQHEwdSZWRtb25kMR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xJjAkBgNVBAMTHU1TTiBDb250ZW50IEF1dGhlbnRpY2F0aW9uIENBMB4XDTA2MDQwMTIwMDI0NVoXDTA2MDcwMTIwMTI0NVowUTESMBAGA1UEChMJTWljcm9zb2Z0MQwwCgYDVQQLEwNNU04xLTArBgNVBAMTJDM0ZmE4MmIyLWZkYTAtNDhkYS04Zjk1LWZjNjBkNWJhYjgyOTCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA45cPz9tVdVnx4ATC0sXxMKMfpzOXvs6qs1d/Z8Pcp3Wr2ovHTd/pRd6Vn8ss/MqTL3hDPxaV+4w4TJCpfoDiCH1H4lwoshw0dY2/eOiWJgd2ONyiJ7dEvStCqrs+QliZVEaGwDjlsh17pHOrBRAA6WBo7TIeiTANpjLn+HkJm80CAwEAAaOCA+IwggPeMB0GA1UdDgQWBBT7ea5Y7aSMXkVnAEDgvXadh5LVSzAfBgNVHSUEGDAWBggrBgEFBQcDCAYKKwYBBAGCNzMBAzCCAksGA1UdIASCAkIwggI+MIICOgYJKwYBBAGCNxUvMIICKzBJBggrBgEFBQcCARY9aHR0cHM6Ly93d3cubWljcm9zb2Z0LmNvbS9wa2kvc3NsL2Nwcy9NaWNyb3NvZnRNU05Db250ZW50Lmh0bTCCAdwGCCsGAQUFBwICMIIBzh6CAcoATQBpAGMAcgBvAHMAbwBmAHQAIABkAG8AZQBzACAAbgBvAHQAIAB3AGEAcgByAGEAbgB0ACAAbwByACAAYwBsAGEAaQBtACAAdABoAGEAdAAgAHQAaABlACAAaQBuAGYAbwByAG0AYQB0AGkAbwBuACAAZABpAHMAcABsAGEAeQBlAGQAIABpAG4AIAB0AGgAaQBzACAAYwBlAHIAdABpAGYAaQBjAGEAdABlACAAaQBzACAAYwB1AHIAcgBlAG4AdAAgAG8AcgAgAGEAYwBjAHUAcgBhAHQAZQAsACAAbgBvAHIAIABkAG8AZQBzACAAaQB0ACAAbQBhAGsAZQAgAGEAbgB5ACAAZgBvAHIAbQBhAGwAIABzAHQAYQB0AGUAbQBlAG4AdABzACAAYQBiAG8AdQB0ACAAdABoAGUAIABxAHUAYQBsAGkAdAB5ACAAbwByACAAcwBhAGYAZQB0AHkAIABvAGYAIABkAGEAdABhACAAcwBpAGcAbgBlAGQAIAB3AGkAdABoACAAdABoAGUAIABjAG8AcgByAGUAcwBwAG8AbgBkAGkAbgBnACAAcAByAGkAdgBhAHQAZQAgAGsAZQB5AC4AIDALBgNVHQ8EBAMCB4AwgaEGA1UdIwSBmTCBloAUdeBjdZAOPzN4/ah2f6tTCLPcC+qhcqRwMG4xCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpXYXNoaW5ndG9uMRAwDgYDVQQHEwdSZWRtb25kMR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xGDAWBgNVBAMTD01TTiBDb250ZW50IFBDQYIKYQlx2AABAAAABTBLBgNVHR8ERDBCMECgPqA8hjpodHRwOi8vY3JsLm1pY3Jvc29mdC5jb20vcGtpL2NybC9wcm9kdWN0cy9NU05Db250ZW50Q0EuY3JsME8GCCsGAQUFBwEBBEMwQTA/BggrBgEFBQcwAoYzaHR0cDovL3d3dy5taWNyb3NvZnQuY29tL3BraS9jZXJ0cy9NU05Db250ZW50Q0EuY3J0MA0GCSqGSIb3DQEBBQUAA4IBAQA6dVva4YeB983Ipos+zhzYfTAz4Rn1ZI7qHrNbtcXCCio/CrKeC7nDy/oLGbgCCn5wAYc4IEyQy6H+faXaeIM9nagqn6bkZHZTFiuomK1tN4V3rI8M23W8PvRqY4kQV5Qwfbz8TVhzEIdMG2ByoK7n9Fq0//kSLLoLqqPmC07oIcGNJPKDGxFzs/5FNEGyIybtmbIEeHSCJGKTDDAOnZAw6ji0873e2WIQsGBUm4VJN153xZgbnmdokWBfutkia6fnTUpcwofGolOe52fMYHYqaccxkP0vnmDGvloSPKOyXpc3RmI6g1rF7VzCQt290jG7A8+yb7OwM+rDooYMj4myMYIBkDCCAYwCAQEwgYowfDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCldhc2hpbmd0b24xEDAOBgNVBAcTB1JlZG1vbmQxHjAcBgNVBAoTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEmMCQGA1UEAxMdTVNOIENvbnRlbnQgQXV0aGVudGljYXRpb24gQ0ECCgmWGQYAAQAAANgwCQYFKw4DAhoFAKBdMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8XDTA2MDYyMzA4NTkzNVowIwYJKoZIhvcNAQkEMRYEFMni2bnV4P6Y9aUW5pzpPmz4hoU3MA0GCSqGSIb3DQEBAQUABIGApK4cGSUKvZiNT7GynJYEfIaSX/UuXf3wJF8cQd7AAy/ULnziD74KUgHfgqMr0h3U+dxbf14e/w6heQdf1Osq3Y+jNvPjhPqAAtIkcMRcgyYiOr973D6u7V5sbp6hKTa74bFVS5bg3ES55vBnAI58IL1JF5Y6qh64lRfhyYjmjjM=";
            session->SendWink( "test1", k );
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
            session->SendFile( "1.txt", "" );
        }
        else if ( cmd == "p4" )
        {
            session->SendActivity( "http://www.xiaoi.com", "INCE SP HOME PAGE" );
        }
        else if ( cmd == "webcam" )
        {
            session->SendWebcam( "192.168.1.26", 81, 101, 9001 );
        }
        else if ( cmd == "name" )
        {
            if ( param.empty() )
                param = "name-" + RandStr();

            m_server->SetDisplayName( session->robot, param.c_str() );
        }
        else if ( cmd == "psm" )
        {
            if ( param.empty() )
                param = "psm-" + RandStr();

            m_server->SetPersonalMessage( session->robot, param.c_str() );
        }
        else if ( cmd == "dp" )
        {
            m_server->SetDisplayPicture( "", "__default.dat" );
        }
        else if ( cmd == "ddp" )
        {
            m_server->SetDisplayPictureEx( "", "__default.dat", "123.cab" );
        }
        else if ( cmd == "scene" )
        {
            m_server->SetScene( "incetest3004@hotmail.com", "dota.jpg" );
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
            m_server->RequestContactList( session->robot );
        }
        else if ( cmd == "push" )
        {
            if ( !param.empty() )
                m_server->PushMessage( session->robot, param.c_str(), "hello" );
        }
        else
        {
            std::string text = "fontname: " + message->FontName + "\r";
            text += "fontstyle: " + NumToStr(message->FontStyle) + "\r";
            text += "fontcolor: 0x" + HexToStr(0xFFFFFF & message->FontColor, 6) + "\r";
            text += "message: " + message->Text + "\r";
            session->SendText( text.c_str() );
        }
        
        END_
    }

    HRESULT __stdcall OnNudgeReceived(IRobotSession* session)
    {
        PRINT_NULL(OnNudgeReceived);
    }

    HRESULT __stdcall OnActivityAccepted(IRobotSession* session)
    {
        PRINT_NULL(OnActivityAccepted);
    }

    HRESULT __stdcall OnActivityRejected(IRobotSession* session)
    {
        PRINT_NULL(OnActivityRejected);
    }

    HRESULT __stdcall OnActivityReceived(IRobotSession* session, BSTR data)
    {
        BEGIN_(OnActivityReceived)
        PRINT_EVENT(std::string("data=") + BSTR2A(data));
        END_
    }

    HRESULT __stdcall OnActivityLoaded(IRobotSession* session)
    {
        PRINT_NULL(OnActivityLoaded);
    }

    HRESULT __stdcall OnActivityClosed(IRobotSession* session)
    {
        PRINT_NULL(OnActivityClosed);
    }

    HRESULT __stdcall OnUserJoined(IRobotSession* session, IRobotUser* user)
    {
        PRINT_NULL(OnUserJoined);
    }

    HRESULT __stdcall OnUserLeft(IRobotSession* session, IRobotUser* user)
    {
        PRINT_NULL(OnUserLeft);
    }

    HRESULT __stdcall OnUserAdd(BSTR robot, BSTR user)
    {
        BEGIN_(OnUserAdd)
        PRINT_EVENT(std::string("robot=") + BSTR2A(robot) + ",user=" + BSTR2A(user));
        END_
    }

    HRESULT __stdcall OnUserRemove(BSTR robot, BSTR user)
    {
        BEGIN_(OnUserRemove)
        PRINT_EVENT(std::string("robot=") + BSTR2A(robot) + ",user=" + BSTR2A(user));
        END_
    }

    HRESULT __stdcall OnExceptionCaught(IRobotSession* session, LONG cause)
    {
        BEGIN_(OnExceptionCaught)
        PRINT_EVENT( "cause=" + NumToStr(cause) );
        END_
    }

    HRESULT __stdcall OnFileAccepted(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(OnFileAccepted)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + NumToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall OnFileRejected(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(OnFileRejected)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + NumToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall OnFileTransferEnded(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(OnFileTransferEnded)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + NumToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall OnFileTransferCancelled(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(OnFileTransferCancelled)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + NumToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall OnFileTransferError(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(OnFileTransferError)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + NumToStr(fileDescriptor->Size) );
        END_
    }

    HRESULT __stdcall OnFileInvited(IRobotSession* session, IRobotFileDescriptor* fileDescriptor)
    {
        BEGIN_(OnFileInvited)
        PRINT_EVENT( std::string("transferId") + fileDescriptor->transferId + "name=" + fileDescriptor->Name + ",size=" + NumToStr(fileDescriptor->Size) + ",thumbnail=" + fileDescriptor->Thumbnail );
        END_
    }

    HRESULT __stdcall OnWebcamAccepted(IRobotSession* session)
    {
        PRINT_NULL(OnWebcamAccepted);
    }

    HRESULT __stdcall OnWebcamRejected(IRobotSession* session)
    {
        PRINT_NULL(OnWebcamRejected);
    }

    HRESULT __stdcall OnWebcamClosed(IRobotSession* session)
    {
        PRINT_NULL(OnWebcamClosed);
    }

    HRESULT __stdcall OnWebcamError(IRobotSession* session)
    {
        PRINT_NULL(OnWebcamError);
    }

    HRESULT __stdcall OnUserUpdated(BSTR robot, IRobotUser* user)
    {
        BEGIN_(OnUserUpdated)
        PRINT_EVENT("robot=" + BSTR2A(robot) + ",friendly=" + user->friendlyName + ",status=" + user->Status);
        END_
    }

    HRESULT __stdcall OnPersonalMessageUpdated(BSTR robot, BSTR user, BSTR personalMessage)
    {
        BEGIN_(OnPersonalMessageUpdated)
        PRINT_EVENT("robot=" + BSTR2A(robot) + ",user=" + BSTR2A(user) + ",psm=" + BSTR2A(personalMessage));
        END_
    }

    HRESULT __stdcall OnContactListReceived(BSTR robot, IRobotUsers* contactList)
    {
        BEGIN_(OnContactListReceived)

        size_t count = contactList->Count();
        PRINT_EVENT( "robot=" + BSTR2A(robot) + ", size=" + NumToStr(count) );
        
        for ( size_t i = 0; i < count; ++i )
        {
            IRobotUserPtr user = contactList->Item(i);
            std::cout << user->friendlyName << std::endl;
        }

        END_
    }

    HRESULT __stdcall OnInkReceived(IRobotSession* session, BSTR ink)
    {
        BEGIN_(OnInkReceived)
        PRINT_EVENT( "size=" + NumToStr(SysStringLen(ink)) );
        session->SendInk( ink );
        END_
    }

    HRESULT __stdcall OnWinkReceived(IRobotSession* session, IRobotResource* resource)
    {
        BEGIN_(OnWinkReceived)
        PRINT_EVENT( "name=" + resource->Name + ",size=" + NumToStr(resource->Size) );
        END_
    }

    HRESULT __stdcall OnVoiceclipReceived(IRobotSession* session, IRobotResource* resource)
    {
        BEGIN_(OnVoiceclipReceived)
        PRINT_EVENT( "name=" + resource->Name + ",size=" + NumToStr(resource->Size) );
        END_
    }

    HRESULT __stdcall OnTypingReceived(IRobotSession* session)
    {
        PRINT_NULL(OnTypingReceived);
        return S_OK;
    }

    HRESULT __stdcall OnSceneUpdated(BSTR robot, BSTR user, IRobotResource* resource)
    {
        BEGIN_(OnSceneUpdated)
        PRINT_EVENT( "robot=" + BSTR2A(robot) + ",user=" + BSTR2A(user) + ",resname=" + resource->Name );
        END_
    }

    HRESULT __stdcall OnDisplayPictureUpdated(BSTR robot, BSTR user, IRobotResource* resource)
    {
        BEGIN_(OnDisplayPictureUpdated)
        PRINT_EVENT( "robot=" + BSTR2A(robot) + ",user=" + BSTR2A(user) + ",resname=" + resource->Name );
        END_
    }

    HRESULT __stdcall OnColorSchemeUpdated(BSTR robot, BSTR user, LONG colorScheme)
    {
        BEGIN_(OnColorSchemeUpdated)
        PRINT_EVENT( "robot=" + BSTR2A(robot) + ",user=" + BSTR2A(user) + ",colorScheme=" + NumToStr(colorScheme) );
        END_
    }


public:
    HRESULT SetServer( IRobotServerPtr server )
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
    spRobotServerFactory->Init( 10 );

    IRobotServerPtr spServer = spRobotServerFactory->CreateRobotServer( /*"192.168.1.44"*//*"192.168.1.174"*/"bottest.com", 6602 );
    RobotServerEventsImpl eventImpl;
    E_IF( eventImpl.SetServer( spServer ) );

    spServer->Login( "SP000125", "test111", 0 );

    MessageBox( NULL, _T("Running"), _T("TEST"), MB_OK );
    
    spServer->Logout();
    spRobotServerFactory->Destroy();
    
    END_
    Sleep(2000);
	return 0;
}

