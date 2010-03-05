Imports System
Imports System.Runtime.InteropServices
Imports BotPlatformSDKLib

Module helloworld

    Sub MessageReceived(ByVal session As IRobotSession, ByVal message As IRobotMessage)
        session.SendText("Hello World!")
        Console.WriteLine(message.Text)
    End Sub

    <MTAThread()> Sub Main()
        Dim robotServerFactory As RobotServerFactory = New RobotServerFactory
        robotServerFactory.Init(2)

        Dim robotServer As RobotServer = robotServerFactory.CreateRobotServer("bottest.com", 6602)
        AddHandler robotServer.MessageReceived, AddressOf MessageReceived
        robotServer.Login("SP000125", "test111", 60000)

        Dim cmd As String = ""
        Do While cmd <> "exit"
            cmd = Console.ReadLine()
        Loop

        robotServer.Logout()
        robotServerFactory.Destroy()
    End Sub

End Module
