Imports System
Imports System.Runtime.InteropServices
Imports BotPlatformSDKLib

Module helloworld

    Sub MessageReceived(ByVal session As IRobotSession, ByVal message As IRobotMessage)

    End Sub

    <MTAThread()> Sub Main()
        Dim robotServerFactory As RobotServerFactory = New RobotServerFactory
        robotServerFactory.Init(2)

        Dim robotServer As RobotServer = robotServerFactory.CreateRobotServer("192.168.1.174", 6602)
        robotServer = robotServerFactory.CreateRobotServer("bottest.com", 6602)
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
