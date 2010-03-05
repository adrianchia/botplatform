using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BotPlatformSDKLib;

namespace botplatform.helloworld
{
    class HelloWorld
    {
        static void Main(string[] args)
        {
            RobotServerFactory serverFactory = new RobotServerFactory();
            serverFactory.Init(2);
            RobotServer server = serverFactory.CreateRobotServer("192.168.1.174", 6602);
            server.MessageReceived += new _IRobotServerEvents_MessageReceivedEventHandler(server_MessageReceived);
            server.Login("SP000125", "test111", 60000);
            string cmd = null;
            while ((cmd = Console.ReadLine()) != null)
                if (cmd.Equals("exit")) break;
            server.Logout();
            serverFactory.Destroy();
        }
        static void server_MessageReceived(IRobotSession session, IRobotMessage message)
        {
            session.SendText("Hello World!");
            Console.WriteLine(message.Text);
        }
    }
}
