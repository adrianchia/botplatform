

Imports System.Runtime.InteropServices
Imports BotPlatformSDKLib


Module Module1

    Sub Main()
        Dim robotServerFactory As New RobotServerFactory
        robotServerFactory.Init(10)

        Dim robotServer As RobotServer = robotServerFactory.CreateRobotServer("bottest.com", 6602)
        robotServer.Login("SP000125", "test111", 0)

        Dim DTitleChangeE As New _IRobotServerEvents_SessionOpened(AddressOf SessionOpened)


        MsgBox("Running", MsgBoxStyle.OkOnly, "TestVB")

        robotServer.Logout()
        robotServerFactory.Destroy()
    End Sub

    Sub SessionOpened(ByVal o As Object)

    End Sub


End Module
