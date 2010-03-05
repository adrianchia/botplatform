using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BotPlatformSDKLib;

namespace botplatform.example
{
    class Example
    {
        static RobotServerFactory serverFactory;
        static RobotServer server;

        static void Main(string[] args)
        {
            string host = "192.168.1.174";
            int port = 6602;
            string spid = "SP000125";
            string sppwd = "test111";

            serverFactory = new RobotServerFactory();
            serverFactory.Init(2);
            server = serverFactory.CreateRobotServer(host, port);
            RegisterEvent(server);
            server.Login(spid, sppwd,60000);

            string cmd = null;
            while ((cmd = Console.ReadLine()) != null)
            {
                if (cmd.Equals("exit"))
                    break;
            }
            server.Logout();
            serverFactory.Destroy();
        }

        static void RegisterEvent(RobotServer server) 
        {
            server.MessageReceived += new _IRobotServerEvents_MessageReceivedEventHandler(server_MessageReceived);
            server.SessionOpened += new _IRobotServerEvents_SessionOpenedEventHandler(server_SessionOpened);
            server.SessionClosed += new _IRobotServerEvents_SessionClosedEventHandler(server_SessionClosed);
            server.TypingReceived += new _IRobotServerEvents_TypingReceivedEventHandler(server_TypingReceived);
            server.NudgeReceived += new _IRobotServerEvents_NudgeReceivedEventHandler(server_NudgeReceived);
            server.InkReceived += new _IRobotServerEvents_InkReceivedEventHandler(server_InkReceived);
            server.WinkReceived += new _IRobotServerEvents_WinkReceivedEventHandler(server_WinkReceived);
            server.VoiceclipReceived += new _IRobotServerEvents_VoiceclipReceivedEventHandler(server_VoiceclipReceived);
            server.ActivityAccepted += new _IRobotServerEvents_ActivityAcceptedEventHandler(server_ActivityAccepted);
            server.ActivityClosed += new _IRobotServerEvents_ActivityClosedEventHandler(server_ActivityClosed);
            server.ActivityLoaded += new _IRobotServerEvents_ActivityLoadedEventHandler(server_ActivityLoaded);
            server.ActivityReceived += new _IRobotServerEvents_ActivityReceivedEventHandler(server_ActivityReceived);
            server.ActivityRejected += new _IRobotServerEvents_ActivityRejectedEventHandler(server_ActivityRejected);
            server.FileInvited += new _IRobotServerEvents_FileInvitedEventHandler(server_FileInvited);
            server.FileAccepted += new _IRobotServerEvents_FileAcceptedEventHandler(server_FileAccepted);
            server.FileRejected += new _IRobotServerEvents_FileRejectedEventHandler(server_FileRejected);
            server.FileTransferEnded += new _IRobotServerEvents_FileTransferEndedEventHandler(server_FileTransferEnded);
            server.FileTransferCancelled += new _IRobotServerEvents_FileTransferCancelledEventHandler(server_FileTransferCancelled);
            server.FileTransferError += new _IRobotServerEvents_FileTransferErrorEventHandler(server_FileTransferError);
            server.UserJoined += new _IRobotServerEvents_UserJoinedEventHandler(server_UserJoined);
            server.UserLeft += new _IRobotServerEvents_UserLeftEventHandler(server_UserLeft);
            server.UserAdd += new _IRobotServerEvents_UserAddEventHandler(server_UserAdd);
            server.UserRemove += new _IRobotServerEvents_UserRemoveEventHandler(server_UserRemove);
            server.UserUpdated += new _IRobotServerEvents_UserUpdatedEventHandler(server_UserUpdated);
            server.PersonalMessageUpdated += new _IRobotServerEvents_PersonalMessageUpdatedEventHandler(server_PersonalMessageUpdated);
            server.DisplayPictureUpdated += new _IRobotServerEvents_DisplayPictureUpdatedEventHandler(server_DisplayPictureUpdated);
            server.SceneUpdated += new _IRobotServerEvents_SceneUpdatedEventHandler(server_SceneUpdated);
            server.ColorSchemeUpdated += new _IRobotServerEvents_ColorSchemeUpdatedEventHandler(server_ColorSchemeUpdated);
            server.ContactListReceived += new _IRobotServerEvents_ContactListReceivedEventHandler(server_ContactListReceived);
            server.ExceptionCaught += new _IRobotServerEvents_ExceptionCaughtEventHandler(server_ExceptionCaught);
        }

        static void server_SessionClosed(IRobotSession session)
        {
            DebugEvent();
        }
        static void server_SessionOpened(IRobotSession session)
        {
            DebugEvent("user=" + session.Users.Item(0).ID + ",friendly="
                + session.Users.Item(0).FriendlyName + ",clientId="
                + session.Users.Item(0).ClientID + ",status="
                + session.Users.Item(0).Status);
            if (OPEN_MODE.OPEN_MODE_CONV_OPEN.Equals(session.OpenMode))
                session.SendText(CommandList);
        }
        static void server_MessageReceived(IRobotSession session, IRobotMessage message)
        {
            DebugEvent("message=" + message.Text);

            string command = message.Text;
            string param = null;
            int ii = command.IndexOf(":");
            if (ii > 0)
            {
                param = command.Substring(ii + 1);
                command = command.Substring(0, ii);
            }

            if ("help".Equals(command)
                    || "?".Equals(command))
            {
                session.SendText(CommandList);
            }
            else if ("preface".Equals(command))
            {
                IRobotMessage msg = session.CreateMessage();
                msg.Signature = "preface-" + new Random().Next(100);
                msg.Text = "test change preface";
                session.SendIM(msg);
            }
            else if ("emo".Equals(command))
            {
                IRobotMessage msg = session.CreateMessage();
                msg.RegisterEmoticon("(1)", "bear.png");
                msg.RegisterEmoticon("(2)", "beaver.png");
                msg.RegisterEmoticon("(3)", "balloon.png");
                msg.Text = "a(1)b(2)c(3)d";
                session.SendIM(msg);
            }
            else if ("typing".Equals(command))
            {
                session.SendTyping();
            }
            else if ("nudge".Equals(command))
            {
                session.SendNudge();
            }
            else if ("wink".Equals(command))
            {
                String k = "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";
                session.SendWink("wink.dat", k);
            }
            else if ("ink".Equals(command))
            {
                session.SendInk("ink.dat");
            }
            else if ("voice".Equals(command))
            {
                session.SendVoiceclip("voiceclip.wav");
            }
            else if ("file".Equals(command))
            {
                session.SendFile("file.txt","testfile");
            }
            else if ("p4".Equals(command))
            {
                session.SendActivity("http://botplatform.com", "botplatform home");
            }
            else if ("webcam".Equals(command))
            {
                session.SendWebcam("192.168.1.26", 81, 101, 9001);
            }
            else if ("name".Equals(command))
            {
                if (param == null) param = "name-" + new Random().Next(1000);
                server.SetDisplayName(session.Robot, param);
            }
            else if ("psm".Equals(command))
            {
                if (param == null) param = "psm-" + new Random().Next(1000);
                server.SetPersonalMessage(session.Robot, param);
            }
            else if ("dp".Equals(command))
            {
                server.SetDisplayPicture(null,"dp.png");
            }
            else if ("ddp".Equals(command))
            {
                server.SetDisplayPictureEx(null,"dp.png", "ddp.dat");
            }
            else if ("scene".Equals(command))
            {
                server.SetScene(null,"scene.png");
            }
            else if ("color".Equals(command))
            {
                server.SetColorScheme(null,0xFF0000);
            }
            else if ("invite".Equals(command))
            {
                if (param != null)
                {
                    session.InviteUser(param);
                }
            }
            else if ("fl".Equals(command))
            {
                server.RequestContactList(session.Robot);
            }
            else if ("push".Equals(command))
            {
                if (param != null)
                {
                    server.PushMessage(session.Robot, param, "hello");
                }
            }
            else if ("create".Equals(command))
            {
                if (param != null)
                    server.CreateSession(session.Robot, param);
            }
            else if ("close".Equals(command))
            {
                session.Close();
            }
            else
            {
                string text = "FontName: " + message.FontName + "\r";
                text = text + "FontStyle: " + message.FontStyle + "\r";
                string hex = ""+message.FontColor;////int..toHexString((0xFFFFFF & message.FontColor));
                for (int k = hex.Length; k < 6; k++) hex = "0" + hex;
                text = text + "FontColor: 0x" + hex + "\r";
                text = text + "Text: " + message.Text + "\r";
                session.SendText(text);
            }

        }
        static void server_TypingReceived(IRobotSession session)
        {
            DebugEvent();
        }
        static void server_NudgeReceived(IRobotSession session)
        {
            DebugEvent();
        }
        static void server_InkReceived(IRobotSession session, string ink)
        {
            DebugEvent("size=" + ink.Length);
            session.SendInk(ink);
        }
        static void server_WinkReceived(IRobotSession session, IRobotResource resource)
        {
            DebugEvent("name=" + resource.Name + ",size=" + resource.Size);
        }
        static void server_VoiceclipReceived(IRobotSession session, IRobotResource resource)
        {
            DebugEvent("name=" + resource.Name + ",size=" + resource.Size);
        }
        static void server_ActivityAccepted(IRobotSession session)
        {
            DebugEvent();
        }
        static void server_ActivityClosed(IRobotSession session)
        {
            DebugEvent();
        }
        static void server_ActivityLoaded(IRobotSession session)
        {
            DebugEvent();
        }
        static void server_ActivityReceived(IRobotSession session, string data)
        {
            DebugEvent("data=" + data);
        }
        static void server_ActivityRejected(IRobotSession session)
        {
            DebugEvent();
        }
        static void server_FileInvited(IRobotSession session, IRobotFileDescriptor fileDescriptor)
        {
            DebugEvent("name=" + fileDescriptor.Name + ",size=" + fileDescriptor.Size);
            session.SendFileRejection(fileDescriptor.transferId);
        }
        static void server_FileAccepted(IRobotSession session, IRobotFileDescriptor fileDescriptor)
        {
            DebugEvent("name=" + fileDescriptor.Name + ",size=" + fileDescriptor.Size);
        }
        static void server_FileRejected(IRobotSession session, IRobotFileDescriptor fileDescriptor)
        {
            DebugEvent("name=" + fileDescriptor.Name + ",size=" + fileDescriptor.Size);
        }
        static void server_FileTransferEnded(IRobotSession session, IRobotFileDescriptor fileDescriptor)
        {
            DebugEvent("name=" + fileDescriptor.Name + ",size=" + fileDescriptor.Size);
        }
        static void server_FileTransferCancelled(IRobotSession session, IRobotFileDescriptor fileDescriptor)
        {
            DebugEvent("name=" + fileDescriptor.Name + ",size=" + fileDescriptor.Size);
        }
        static void server_FileTransferError(IRobotSession session, IRobotFileDescriptor fileDescriptor)
        {
            DebugEvent("name=" + fileDescriptor.Name + ",size=" + fileDescriptor.Size);
        }
        static void server_UserJoined(IRobotSession session, IRobotUser user)
        {
            DebugEvent();
        }
        static void server_UserLeft(IRobotSession session, IRobotUser user)
        {
            DebugEvent();
        }
        static void server_UserAdd(string robot, string user)
        {
            DebugEvent("robot=" + robot + ",user=" + user);
        }
        static void server_UserRemove(string robot, string user)
        {
            DebugEvent("robot=" + robot + ",user=" + user);
        }
        static void server_UserUpdated(string robot, IRobotUser user)
        {
            DebugEvent("robot=" + robot + ",friendly=" + user.FriendlyName + ",status=" + user.Status);
        }
        static void server_PersonalMessageUpdated(string robot, string user, string personalMessage)
        {
            DebugEvent("robot=" + robot + ",user=" + user + ",psm=" + personalMessage);
        }
        static void server_DisplayPictureUpdated(string robot, string user, IRobotResource resource)
        {
            DebugEvent("robot=" + robot + ",user=" + user + ",resname=" + resource.Name);
        }
        static void server_SceneUpdated(string robot, string user, IRobotResource resource)
        {
            DebugEvent("robot=" + robot + ",user=" + user + ",resname=" + resource.Name);
        }
        static void server_ColorSchemeUpdated(string robot, string user, int colorScheme)
        {
            DebugEvent("robot=" + robot + ",user=" + user + ",colorScheme=" + colorScheme);
        }
        static void server_ContactListReceived(string robot, IRobotUsers contactList)
        {
            DebugEvent("robot=" + robot + ", size=" + contactList.Count() + ")");
        }
        static void server_ExceptionCaught(IRobotSession session, int cause)
        {
            throw new NotImplementedException();
        }

        static void DebugEvent()
        {
            DebugEvent(null);
        }

        static void DebugEvent(String info)
        {
		    if (info != null)
			    Console.WriteLine("[EVENT:" + new System.Diagnostics.StackFrame(1).GetMethod().Name + "] " + info);
		    else
                Console.WriteLine("[EVENT:" + new System.Diagnostics.StackFrame(2).GetMethod().Name + "]");
	    }


        public static string CommandList = "*****************************************\r"
            + "**  BOTPLATFORM SDK DEMO COMMAND LIST  **\r"
            + "*****************************************\r"
            + " preface --- test message preface. \r"
            + " emo ------- test emoticon.\r"
            + " typing ---- test typing info.\r"
            + " nudge ----- test nudge.\r"
            + " ink ------- test ink.\r"
            + " wink ------ test wink.\r"
            + " voice ----- test voiceclip.\r"
            + " file ------ test file transfer.\r"
            + " p4 -------- test msn activity.\r"
            + " name ------ set friendly name.\r"
            + " psm ------- set personal message.\r"
            + " dp -------- set display picture.\r"
            + " ddp ------- set delux display picture.\r"
            + " scene ----- set scene.\r"
            + " color ----- set color scheme.\r"
            + " invite ---- invite user.\r"
            + " push ------ push message.\r"
            + " create ---- create session.\r"
            + " close ----- close session.\r"
            + " help ------ print this command list.\r"
            + " ? --------- print this command list.\r";
       
    }

    


}

