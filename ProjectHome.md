<h1><a />What is it?</h1>
<p>Bot Platform lets you create your own robot with your favorite developing languages and environments, such as JAVA C# C++ VB and so on. The SDK is easy to distribute, develop, debug and deploy. Developers can devote their energies to the robot itself, without taking care of the complex communication protocol. The SDK supports almost all the Messenger features, including user file, custom emoticon, nudge, etc. An integrated activity window is also provided in this SDK, so that developers can send user a web page with rich contents in the right activity window.</p>

<h1><a />Documentation</h1>
<p><a href='http://botplatform.com/document.jsp'>Read the documentation.</a> </p>

<h1><a />Discussion</h1>
<p><a href='http://botplatform.uservoice.com/'>Visit the discussion group.</a> </p>

<h1><a />Quick Example</h1>
<p>Java Example: </p>
```

RobotServerFactory serverFactory = RobotServerFactory.getInstance();
final RobotServer server = serverFactory.createRobotServer("server.botplatform.com", 6602);
server.setReconnectedSupport(true);
server.setRobotHandler(new RobotAdapter(){
public void messageReceived(RobotSession session,
RobotMessage message) {
session.send("Hello World!");
System.out.println(message.getString());
}
});
server.login("you spid", "your sppwd");
Runtime.getRuntime().addShutdownHook(new Thread(){
public void run(){
server.logout();
}
});
```
<p>C# Example: </p>
```

static void server_MessageReceived(IRobotSession session, IRobotMessage message)
{
session.SendText("Hello World!");
Console.WriteLine(message.Text);
}
static void Main(string[] args)
{
RobotServerFactory serverFactory = new RobotServerFactory();
serverFactory.Init(2);
RobotServer server = serverFactory.CreateRobotServer("server.botplatform.com", 6602);
server.MessageReceived += new _IRobotServerEvents_MessageReceivedEventHandler(server_MessageReceived);
server.Login("your sppid", "your sppwd", 60000);
string cmd = null;
while ((cmd = Console.ReadLine()) != null)
if (cmd.Equals("exit")) break;
server.Logout();
serverFactory.Destroy();
}
```
<p>C++ Example: </p>
```

_ATL_FUNC_INFO  s_info_onMessageReceived = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };
class RobotServerEventsImpl : public IDispEventSimpleImpl<1, RobotServerEventsImpl, &DIID__IRobotServerEvents>
{
public:
BEGIN_SINK_MAP(RobotServerEventsImpl)
SINK_ENTRY_INFO(1, DIID__IRobotServerEvents, 3, &RobotServerEventsImpl::onMessageReceived, &s_info_onMessageReceived)
END_SINK_MAP()

HRESULT __stdcall onMessageReceived( IRobotSession* session, IRobotMessage* message )
{
session->SendText("Hello World!");
std::cout << message->Text << std::endl;
return S_OK;
}
};
int _tmain(int argc, _TCHAR* argv[])
{
CoInitializeEx(NULL, COINIT_MULTITHREADED);
{
CComPtr<IRobotServerFactory> spRobotServerFactory;
spRobotServerFactory.CoCreateInstance( CLSID_RobotServerFactory, NULL, CLSCTX_INPROC );
spRobotServerFactory->Init( 2 );

IRobotServerPtr spRobotServer = spRobotServerFactory->CreateRobotServer( "server.botplatform.com", 6602 );
RobotServerEventsImpl eventImpl;
eventImpl.DispEventAdvise( spRobotServer );
spRobotServer->Login( "your spid", "your sppwd", 60000 );

std::string cmd;
while ( true )
{
std::cin >> cmd;
if ( cmd == "exit" )
break;
}
spRobotServer->Logout();
spRobotServerFactory->Destroy();
}

CoUninitialize();
return 0;
}
```
<p>VB Example: </p>
```

Sub MessageReceived(ByVal session As IRobotSession, ByVal message As IRobotMessage)
session.SendText("Hello World!")
Console.WriteLine(message.Text)
End Sub
<MTAThread()> Sub Main()
Dim robotServerFactory As RobotServerFactory = New RobotServerFactory
robotServerFactory.Init(2)

Dim robotServer As RobotServer = robotServerFactory.CreateRobotServer("server.botplatform.com", 6602)
AddHandler robotServer.MessageReceived, AddressOf MessageReceived
robotServer.Login("your spid", "your sppwd", 60000)

Dim cmd As String = ""
Do While cmd <> "exit"
cmd = Console.ReadLine()
Loop

robotServer.Logout()
robotServerFactory.Destroy()
End Sub
```
<p>For a more complete example, see the <a href='http://botplatform.com/document.jsp'>tutorials</a>. </p>