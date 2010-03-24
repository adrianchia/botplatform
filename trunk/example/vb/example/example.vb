Imports System
Imports System.Runtime.InteropServices
Imports BotPlatformSDKLib

Module example

    Dim m_server As RobotServer

    Function GetCommandList() As String
        GetCommandList = _
            "*****************************************" + vbCr + _
            "**  BOTPLATFORM SDK DEMO COMMAND LIST  **" + vbCr + _
            "*****************************************" + vbCr + _
            " preface --- test message preface. " + vbCr + _
            " emo ------- test emoticon." + vbCr + _
            " typing ---- test typing info." + vbCr + _
            " nudge ----- test nudge." + vbCr + _
            " ink ------- test ink." + vbCr + _
            " wink ------ test wink." + vbCr + _
            " voice ----- test voiceclip." + vbCr + _
            " file ------ test file transfer." + vbCr + _
            " p4 -------- test msn activity." + vbCr + _
            " name ------ set friendly name." + vbCr + _
            " psm ------- set personal message." + vbCr + _
            " dp -------- set display picture." + vbCr + _
            " ddp ------- set deluxe display picture." + vbCr + _
            " scene ----- set scene." + vbCr + _
            " color ----- set color scheme." + vbCr + _
            " invite ---- invite user." + vbCr + _
            " help ------ print this command list." + vbCr + _
            " ? --------- print this command list." + vbCr
    End Function

    Function GetRandStr() As String
        GetRandStr = CStr(Int(100 * Rnd()))
    End Function

    Function HexToStr(ByVal num As Object, ByVal padding As Integer) As String
        Dim str As String = Hex(num)

        Do While str.Length < 6
            str = "0" + num
        Loop

        HexToStr = str
    End Function

    Sub DebugEvent(ByVal eveName As String, Optional ByVal info As String = Nothing)
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

        AddHandler robotServer.UserUpdated, AddressOf UserUpdated
        AddHandler robotServer.PersonalMessageUpdated, AddressOf PersonalMessageUpdated

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
            DebugEvent("SessionOpened", "user=" + session.Users.Item(0).ID + _
                    ",friendly=" + session.Users.Item(0).FriendlyName + _
                    ",clientId=" + CStr(session.Users.Item(0).ClientID) + _
                    ",status=" + session.Users.Item(0).Status)

            If OPEN_MODE.OPEN_MODE_CONV_OPEN = session.OpenMode Then
                session.SendText(GetCommandList())
            End If

        Catch e As Exception
            Console.WriteLine(e.Message)
        End Try
    End Sub

    Sub SessionClosed(ByVal session As IRobotSession)
        DebugEvent("onSessionClosed")
    End Sub

    Sub MessageReceived(ByVal session As IRobotSession, ByVal message As IRobotMessage)
        Try
            Dim cmd As String = message.Text
            Dim param As String = Nothing
            Dim pos As Integer

            DebugEvent("MessageReceived", "message=" + cmd)

            pos = cmd.IndexOf(":")
            If pos >= 0 Then
                param = cmd.Substring(pos + 1)
                cmd = cmd.Substring(0, pos)
            End If

            If cmd = "help" Or cmd = "?" Then
                session.SendText(GetCommandList())
            ElseIf cmd = "preface" Then
                Dim msg As IRobotMessage = session.CreateMessage()
                msg.Signature = "preface-" + GetRandStr()
                msg.Text = "test change preface"
                session.SendIM(msg)
            ElseIf cmd = "emo" Then
                Dim msg As IRobotMessage = session.CreateMessage()
                msg.RegisterEmoticon("(1)", "emo1.png")
                msg.RegisterEmoticon("(2)", "emo2.png")
                msg.RegisterEmoticon("(3)", "emo3.png")
                msg.RegisterEmoticon("(4)", "emo4.png")
                msg.RegisterEmoticon("(5)", "emo5.png")
                msg.Text = "a(1)b(2)c(3)d(4)e(5)f"
                session.SendIM(msg)
            ElseIf cmd = "typing" Then
                session.SendTyping()
            ElseIf cmd = "nudge" Then
                session.SendNudge()
            ElseIf cmd = "wink" Then
                Dim k As String = "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"
                session.SendWink("wink.cab", k)
            ElseIf cmd = "ink" Then
                session.SendInk("AMgCHAOAgAQdA9ABbgMESBFFZBkUMggAgBACAAAAQjMIAIAMAgAAAEIVq6rTQauq00EAACq9AAAAvx4HCIL8UficAAoXGoL8qflUAAAAgvwR+CcvLYJstllLmwAKFBKC/Kn5TuWblMtlmoL8efj0ALAACholgv0J+hQAAAAAAIL8DfgUlJUVFly2FJSxQAo/aoL9QfqGBYsqUAWFRUspLLKiwAAAACUCVJuUsssAgvyZ+TQLCpZZUsspLKgsAllliyyxYWWWLKSykssKJQAAChcbgv2R+yQAWAAAgvwN+B8JYWJsbCrLAAoWGoL9zfucAAAAgvwR+CYlhLLmspTZQAo3S4L+AAP4ABssFgASyyxZc2WXNlliygAVLFCWAIL8cfje5bLFly2BZZYoAssVKlllJUsspLLLAAoQCoL+ACP4AJssqIL8bfjcAA==")
            ElseIf cmd = "voice" Then
                session.SendVoiceclip("voiceclip.wav")
            ElseIf cmd = "file" Then
                session.SendFile("file.pdf", "BotPlatform SDK Protocol.pdf")
            ElseIf cmd = "p4" Then
                session.SendActivity("http://botplatform.com", "botplatform home")
            ElseIf cmd = "name" Then
                If IsNothing(param) Then
                    param = "name-" + GetRandStr()
                End If
                m_server.SetDisplayName(session.Robot, param)
            ElseIf cmd = "psm" Then
                If IsNothing(param) Then
                    param = "psm-" + GetRandStr()
                End If
                m_server.SetPersonalMessage(session.Robot, param)
            ElseIf cmd = "dp" Then
                m_server.SetDisplayPicture(Nothing, "dp.png")
            ElseIf cmd = "ddp" Then
                m_server.SetDisplayPictureEx(Nothing, "dp.png", "ddp.cab")
            ElseIf cmd = "scene" Then
                m_server.SetScene(Nothing, "scene.png")
            ElseIf cmd = "color" Then
                m_server.SetColorScheme("", Rnd())
            ElseIf cmd = "invite" Then
                If Not IsNothing(param) Then
                    session.InviteUser(param)
                End If
            Else
                Dim text As String
                text = "fontname: " + message.FontName + "" + vbCr
                text += "fontstyle: " + CStr(message.FontStyle) + "" + vbCr
                text += "fontcolor: 0x" + HexToStr(&HFFFFFF And message.FontColor, 6) + "" + vbCr
                text += "message: " + message.Text + "" + vbCr
                session.SendText(text)
            End If
        Catch e As Exception
            Console.WriteLine(e.Message)
        End Try
    End Sub

    Sub NudgeReceived(ByVal session As IRobotSession)
        DebugEvent("NudgeReceived")
    End Sub

    Sub ActivityAccepted(ByVal session As IRobotSession)
        DebugEvent("ActivityAccepted")
    End Sub

    Sub ActivityRejected(ByVal session As IRobotSession)
        DebugEvent("ActivityRejected")
    End Sub

    Sub ActivityReceived(ByVal session As IRobotSession, ByVal data As String)
        DebugEvent("ActivityReceived", "data=" + data)
    End Sub

    Sub ActivityLoaded(ByVal session As IRobotSession)
        DebugEvent("ActivityLoaded")
    End Sub

    Sub ActivityClosed(ByVal session As IRobotSession)
        DebugEvent("ActivityClosed")
    End Sub

    Sub UserJoined(ByVal session As IRobotSession, ByVal user As IRobotUser)
        DebugEvent("UserJoined")
    End Sub

    Sub UserLeft(ByVal session As IRobotSession, ByVal user As IRobotUser)
        DebugEvent("UserLeft")
    End Sub

    Sub UserAdd(ByVal robot As String, ByVal user As String)
        DebugEvent("UserAdd", "robot=" + robot + ",user=" + user)
    End Sub

    Sub UserRemove(ByVal robot As String, ByVal user As String)
        DebugEvent("UserRemove", "robot=" + robot + ",user=" + user)
    End Sub

    Sub ExceptionCaught(ByVal session As IRobotSession, ByVal cause As Integer)
        DebugEvent("ExceptionCaught", "cause=" + CStr(cause))
    End Sub

    Sub FileAccepted(ByVal session As IRobotSession, ByVal fileDescriptor As IRobotFileDescriptor)
        DebugEvent("FileAccepted", "transferId=" + fileDescriptor.transferId + ",name=" + fileDescriptor.Name + ",size=" + CStr(fileDescriptor.Size))
    End Sub

    Sub FileRejected(ByVal session As IRobotSession, ByVal fileDescriptor As IRobotFileDescriptor)
        DebugEvent("FileRejected", "transferId=" + fileDescriptor.transferId + ",name=" + fileDescriptor.Name + ",size=" + CStr(fileDescriptor.Size))
    End Sub

    Sub FileTransferEnded(ByVal session As IRobotSession, ByVal fileDescriptor As IRobotFileDescriptor)
        DebugEvent("FileTransferEnded", "transferId=" + fileDescriptor.transferId + ",name=" + fileDescriptor.Name + ",size=" + CStr(fileDescriptor.Size))
    End Sub

    Sub FileTransferCancelled(ByVal session As IRobotSession, ByVal fileDescriptor As IRobotFileDescriptor)
        DebugEvent("FileTransferCancelled", "transferId=" + fileDescriptor.transferId + ",name=" + fileDescriptor.Name + ",size=" + CStr(fileDescriptor.Size))
    End Sub

    Sub FileTransferError(ByVal session As IRobotSession, ByVal fileDescriptor As IRobotFileDescriptor)
        DebugEvent("FileTransferCancelled", "transferId=" + fileDescriptor.transferId + ",name=" + fileDescriptor.Name + ",size=" + CStr(fileDescriptor.Size))
    End Sub

    Sub FileInvited(ByVal session As IRobotSession, ByVal fileDescriptor As IRobotFileDescriptor)
        DebugEvent("FileInvited", "transferId=" + fileDescriptor.transferId + ",name=" + fileDescriptor.Name + ",size=" + CStr(fileDescriptor.Size) + ",thumbnail=" + fileDescriptor.Thumbnail)
        session.SendFileRejection(fileDescriptor.transferId)
    End Sub

    Sub UserUpdated(ByVal robot As String, ByVal user As IRobotUser)
        DebugEvent("UserUpdated", "robot=" + robot + ",friendly=" + user.FriendlyName + ",status=" + user.Status)
    End Sub

    Sub PersonalMessageUpdated(ByVal robot As String, ByVal user As String, ByVal personalMessage As String)
        DebugEvent("PersonalMessageUpdated", "robot=" + robot + ",user=" + user + ",psm=" + personalMessage)
    End Sub

    Sub InkReceived(ByVal session As IRobotSession, ByVal ink As String)
        Try
            DebugEvent("InkReceived", "size=" + ink.Length)
            session.SendInk(ink)
        Catch e As Exception
            Console.WriteLine(e.Message)
        End Try
    End Sub

    Sub WinkReceived(ByVal session As IRobotSession, ByVal resource As IRobotResource)
        DebugEvent("WinkReceived", "name=" + resource.Name + ",size=" + CStr(resource.Size))
    End Sub

    Sub VoiceclipReceived(ByVal session As IRobotSession, ByVal resource As IRobotResource)
        DebugEvent("VoiceclipReceived", "name=" + resource.Name + ",size=" + CStr(resource.Size))
    End Sub

    Sub TypingReceived(ByVal session As IRobotSession)
        DebugEvent("TypingReceived")
    End Sub

    Sub SceneUpdated(ByVal robot As String, ByVal user As String, ByVal resource As IRobotResource)
        DebugEvent("SceneUpdated", "robot=" + robot + ",user=" + user + ",resname=" + resource.Name)
    End Sub

    Sub DisplayPictureUpdated(ByVal robot As String, ByVal user As String, ByVal resource As IRobotResource)
        Try
            DebugEvent("DisplayPictureUpdated", "robot=" + robot + ",user=" + user + ",resname=" + resource.Name)
            m_server.RequestResource(robot, user, resource, "")
        Catch e As Exception
            Console.WriteLine(e.Message)
        End Try
    End Sub

    Sub ColorSchemeUpdated(ByVal robot As String, ByVal user As String, ByVal colorScheme As Integer)
        DebugEvent("ColorSchemeUpdated", "robot=" + robot + ",user=" + user + ",colorScheme=" + CStr(colorScheme))
    End Sub

    <MTAThread()> Sub Main()
        Try
            Console.WriteLine("Robot Server Starting...")

            Dim robotServerFactory As RobotServerFactory = New RobotServerFactory
            robotServerFactory.Init(2)

            m_server = robotServerFactory.CreateRobotServer("server.botplatform.com", 6602)
            SetAllEvents(m_server)
            m_server.Login("SP106825", "123qwe", 60000)

            Console.WriteLine("Robot Server Logged In.")

            Dim cmd As String = ""
            Do While cmd <> "exit"
                cmd = Console.ReadLine()
            Loop

            m_server.Logout()
            robotServerFactory.Destroy()

            Console.WriteLine("Robot Server Stopped.")

        Catch e As Exception
            Console.WriteLine(e.Message)
            Console.ReadLine()
        End Try
    End Sub

End Module
