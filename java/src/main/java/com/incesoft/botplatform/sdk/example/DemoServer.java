
package com.incesoft.botplatform.sdk.example;

import com.incesoft.botplatform.sdk.RobotServer;
import com.incesoft.botplatform.sdk.RobotServerFactory;

/**
 * @author LiBo
 */
public class DemoServer {

	public static void main(String[] args) throws Exception {
		
		String host = "192.168.1.174";
		int port = 6602;
		String spid = "SP000125";
		String sppwd = "test111";

		System.out.println("RobotServer starting");
		
		long old = System.currentTimeMillis();
		final RobotServerFactory serverFactory = RobotServerFactory.getInstance();
		final RobotServer server = serverFactory.createRobotServer(host, port);
		server.setReconnectedSupport(true);
		server.setRobotHandler(new MyHandler(server));
		
		System.out.println("RobotServer connect to " + host+":"+port + " ...");
		
		server.login(spid, sppwd);
		
		System.out.println("RobotServer started in " + (System.currentTimeMillis()-old) + "ms");
		
		Runtime.getRuntime().addShutdownHook(new Thread(){
			public void run(){
				server.logout();
			}
		});
		
	}

}
