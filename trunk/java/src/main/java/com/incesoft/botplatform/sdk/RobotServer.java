/*
 * Created on 2006-2-20
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk;


/**
 * Robot server can be created by RobotServerFactory. 
 * Multiple Robot Servers is recommended, when there are large numbers of users. 
 * Robot Servers can run on one or more physical machines.
 * @author LiBo
 */
public interface RobotServer {
    
    /**
     * Calling this function, Robot Server will login BOTPLATFORM. 
     * It's a synchronous operation, using default timeout(1 minute). 
     * During the login process, if no exception raises, login succeeds. 
     * Otherwise, please catch the exception and check exception message explains why login failed.
     * @param spid The unique ID assigned by INCESOFT 
     * @param sppwd The password assigned by INCESOFT
     */
    public void login(String spid, String sppwd) throws RobotException;
    /**
     * Calling this function, Robot Server will login BOTPLATFORM. 
     * It's a synchronous operation, using assigned timeout. 
     * During the login process, if no exception raises, login succeeds. 
     * Otherwise, please catch the exception and check exception message explains why login failed. 
     * @param spid The unique ID assigned by INCESOFT
     * @param sppwd The password assigned by INCESOFT
     * @param timeout Login timeout, the unit is millisecond 
     */
    public void login(String spid, String sppwd, long timeout) throws RobotException;
    
    /**
     * Robot server logout BOTPLATFORM
     */
    public void logout();
    
    /**
     * Set reconnection support. 
     * Default setting is true. 
     * When network errors occur, Robot Server will login BOTPLATFORM again automatically. 
     */
    public void setReconnectedSupport(boolean b);
    /**
     * check whether reconnection is supported .
     */
    public boolean isReconnectedSupport();
    
    /**
     * Set robot server event handler 
     */
    public void setRobotHandler(RobotHandler handler);
    
    /**
     * Add robot server connection listener 
     */
    public void addConnectionListener(RobotConnectionListener listener);
    /**
     * Remove robot server connection listener
     */
    public void removeConnectionistener(RobotConnectionListener listener);
    
    /**
     * Set robot display name
     * @param displayName
     */
    public void setDisplayName(String displayName) throws RobotException;
    
    /**
     * Set a robot display name
     * @param displayName
     */
    public void setDisplayName(String robotAccount, String displayName) throws RobotException ;
    
    /**
     * Set robot personal message
     * @param personalMessage
     */
    public void setPersonalMessage(String personalMessage) throws RobotException;

    /**
     * Set a robot personal message
     * @param personalMessage
     */    
    public void setPersonalMessage(String robotAccount, String personalMessage) throws RobotException;
    
    
    /**
     * Set robot display picture
     * @param displayPicture unique name of the dp resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
     */
    public void setDisplayPicture(String displayPicture) throws RobotException;

    /**
     * Set a robot display picture
     * @param displayPicture unique name of the dp resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
     */    
    public void setDisplayPicture(String robotAccount, String displayPicture) throws RobotException;
    
    /**
     * Set robot display picture
     * @param displayPicture unique name of the dp resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
     * @param largeDisplayPic
     */
    public void setDisplayPictureEx(String displayPicture, String largePicture) throws RobotException;

    /**
     * Set a robot display picture
     * @param robotAccount
     * @param displayPicture unique name of the dp resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
     * @param largeDisplayPic
     */    
    public void setDisplayPictureEx(String robotAccount, String displayPicture, String largePicture) throws RobotException;
    
    /** 
     * Send chat scene request to server.
     * @param scene uri of the scene
     * @throws RobotException
     */
    public void setScene(String scene) throws RobotException ;
    
    /** 
     * Send chat scene request to server.
     * @param robotAccount robot id
     * @param scene uri of the scene
     * @throws RobotException
     */
    public void setScene(String robotAccount, String scene) throws RobotException ;
    
    public void setColorScheme(int colorScheme)  throws RobotException;
    
    public void setColorScheme(String robotAccount, int colorScheme)  throws RobotException;
    
    /**
     * Create a robot session.
     * @param robot robot id
     * @param user user id
     */
    public void createSession(String robot, String user) throws RobotException;
    
    /**
     * Push message to user. may be msn offline message,email message, etc.
     * @param robot robot id
     * @param user user id
     * @param message message
     */
    public void pushMessage(String robot, String user, String message) throws RobotException;
    /** 
     * Send contact list request to server.
     * @param robot robot id
     * @throws RobotException
     */
    public void requestContactList(String robot) throws RobotException ;
    
    /** 
     * Send resource request to server.
     * @param robot robot id
     * @throws RobotException
     */
    public void requestResource(String robot, String user, RobotResource resource, String saveUrl) throws RobotException ;
    
}
