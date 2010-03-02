/*
 * Created on 2006-2-20
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk;

/**
 * Robot server connection listener 
 * @author LiBo
 */
public interface RobotConnectionListener {
    /**
     * Occurs when robot server connects to INCE BOTPLATFORM
     */
    public void serverConnected(RobotServer server);
    
    /**
     * Occurs when robot server reconnects to INCE BOTPLATFORM
     */
    public void serverReconnected(RobotServer server);
    
    /**
     * Occurs when robot server disconnects INCE BOTPLATFORM
     */
    public void serverDisconnected(RobotServer server);
}
