

Imports System.Runtime.InteropServices
Imports BotPlatformSDKLib

Module Module1

    Sub Main()
        Dim robotServerFactory As New RobotServerFactoryClass
        robotServerFactory.Init(10)


        Dim robotServer As RobotServerClass = robotServerFactory.CreateRobotServer("bottest.com", 6602)
        robotServer.Login("SP000125", "test111", 0)


        robotServer.MessageReceived()

+= new _IRobotServerEvents_MessageReceivedEventHandler(server_MessageReceived);


        'Dim DTitleChangeE As New _IRobotServerEvents_SessionOpened(AddressOf SessionOpened)

        MsgBox("Running", MsgBoxStyle.OkOnly, "TestVB")

        robotServer.Logout()
        robotServerFactory.Destroy()
    End Sub

    Sub SessionOpened(ByVal o As Object)

    End Sub


End Module
