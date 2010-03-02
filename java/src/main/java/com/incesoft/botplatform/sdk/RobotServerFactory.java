/*
 * Created on 2006-2-22
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk;

import com.incesoft.botplatform.sdk.support.DefaultRobotServerFactory;

/**
 * Robot server factory to generate robot server instance.
 * @author LiBo
 */
public abstract class RobotServerFactory {
    
    private static RobotServerFactory INSTANCE = new DefaultRobotServerFactory(); 

    /**
     * Get RobotServerFactory instance
     */
    public static RobotServerFactory getInstance() {
        return INSTANCE;
    }
        
    /**
     * Create a RobotServer instance
     * @param ip IP of BOTPLATFORM for RobotServer to login 
     * @param port Port of BOTPLATFORM for RobotServer to login 
     */
    public abstract RobotServer createRobotServer(String ip, int port);
    
    
}
