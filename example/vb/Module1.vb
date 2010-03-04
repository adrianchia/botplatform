
Imports System
Imports System.Runtime.InteropServices
Imports BotPlatformSDKLib

Module Module1
    Sub PrintLn(ByVal eveName As String, Optional ByVal info As String)
        If IsNothing(info) Then
            Console.WriteLine("[EVENT:" + eveName + "]")
        Else
            Console.WriteLine("[EVENT:" + eveName + "]" + info)
        End If
    End Sub

    Sub SetAllEvents(ByVal robotServer As RobotServer)
        AddHandler robotServer.SessionOpened, AddressOf SessionOpened
        AddHandler robotServer.SessionClosed, AddressOf SessionClosed
        AddHandler robotServer.MessageReceived, AddressOf MessageReceived
        AddHandler robotServer.NudgeReceived, AddressOf NudgeReceived

        AddHandler robotServer.ActivityAccepted, AddressOf ActivityAccepted
        AddHandler robotServer.ActivityRejected, AddressOf ActivityRejected
        AddHandler robotServer.ActivityReceived, AddressOf ActivityReceived
        AddHandler robotServer.ActivityLoaded, AddressOf ActivityLoaded
        AddHandler robotServer.ActivityClosed, AddressOf ActivityClosed

        AddHandler robotServer.UserJoined, AddressOf UserJoined
        AddHandler robotServer.UserLeft, AddressOf UserLeft
        AddHandler robotServer.UserAdd, AddressOf UserAdd
        AddHandler robotServer.UserRemove, AddressOf UserRemove

        AddHandler robotServer.ExceptionCaught, AddressOf ExceptionCaught

        AddHandler robotServer.FileAccepted, AddressOf FileAccepted
        AddHandler robotServer.FileRejected, AddressOf FileRejected
        AddHandler robotServer.FileTransferEnded, AddressOf FileTransferEnded
        AddHandler robotServer.FileTransferCancelled, AddressOf FileTransferCancelled
        AddHandler robotServer.FileTransferError, AddressOf FileTransferError
        AddHandler robotServer.FileInvited, AddressOf FileInvited

        AddHandler robotServer.WebcamAccepted, AddressOf WebcamAccepted
        AddHandler robotServer.WebcamRejected, AddressOf WebcamRejected
        AddHandler robotServer.WebcamClosed, AddressOf WebcamClosed
        AddHandler robotServer.WebcamError, AddressOf WebcamError

        AddHandler robotServer.UserUpdated, AddressOf UserUpdated
        AddHandler robotServer.PersonalMessageUpdated, AddressOf PersonalMessageUpdated

        AddHandler robotServer.ContactListReceived, AddressOf ContactListReceived
        AddHandler robotServer.InkReceived, AddressOf InkReceived
        AddHandler robotServer.WinkReceived, AddressOf WinkReceived
        AddHandler robotServer.VoiceclipReceived, AddressOf VoiceclipReceived
        AddHandler robotServer.TypingReceived, AddressOf TypingReceived

        AddHandler robotServer.SceneUpdated, AddressOf SceneUpdated
        AddHandler robotServer.DisplayPictureUpdated, AddressOf DisplayPictureUpdated
        AddHandler robotServer.ColorSchemeUpdated, AddressOf ColorSchemeUpdated

    End Sub

    Sub SessionOpened(ByVal session As IRobotSession)
        Try
            PrintLn()
            BEGIN_(onSessionOpened)

        PRINT_EVENT( std::string("user=") + session->GetUsers()->Item(0)->ID + 
            ",friendly=" + session->GetUsers()->Item(0)->friendlyName + 
            ",clientId=" + numToStr(session->GetUsers()->Item(0)->ClientID) +
            ",status="   + session->GetUsers()->Item(0)->Status );

        if ( OPEN_MODE_CONV_OPEN == session->GetOpenMode() )
        {
            session->SendText( commandList );
        }

        Catch ex As Exception
            Console.WriteLine()
        End Try

    End Sub

    Sub MessageReceived(ByVal session As IRobotSession, ByVal message As IRobotMessage)
        Console.WriteLine(message.Text)
    End Sub

    Sub Main()
        Dim robotServerFactory As New RobotServerFactory
        robotServerFactory.Init(10)

        Dim robotServer As RobotServer = robotServerFactory.CreateRobotServer("bottest.com", 6602)
        robotServer.Login("SP000125", "test111", 0)
        SetAllEvents(robotServer)

        MsgBox("Running", MsgBoxStyle.OkOnly, "TestVB")

        robotServer.Logout()
        robotServerFactory.Destroy()
    End Sub

End Module
