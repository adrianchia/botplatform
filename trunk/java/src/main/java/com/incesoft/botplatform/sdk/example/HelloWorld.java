package com.incesoft.botplatform.sdk.example;

import com.incesoft.botplatform.sdk.RobotAdapter;
import com.incesoft.botplatform.sdk.RobotMessage;
import com.incesoft.botplatform.sdk.RobotServer;
import com.incesoft.botplatform.sdk.RobotServerFactory;
import com.incesoft.botplatform.sdk.RobotSession;

public class HelloWorld {
	
	public static void main(String[] args) throws Exception {
		
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
	}
}
