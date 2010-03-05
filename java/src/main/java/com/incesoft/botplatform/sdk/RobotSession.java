/*
 * Created on 2006-2-20
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk;

import java.util.Collection;
import java.util.Iterator;



/**
 * A session between robot and user. 
 * When user opens a conversation window or reconnect after session timeout, a session starts. 
 * During the session, SP can send message to user. 
 * When users close a conversation window or session timeout, a session ends and SP cannot send any messages. 
 * @author LiBo
 */
public interface RobotSession {
    
    /**
     * A flag indicates that a new conversation started by unkonw reason.
     */
    public static final int OPEN_MODE_UNKONW = 0;
    /**
     * A flag indicates that a new conversation started by user.
     */
    public static final int OPEN_MODE_CONV_OPEN = 1;
    /**
     * A flag indicates that a new conversation started by reconnecting after session timeout.
     */
    public static final int OPEN_MODE_TIMEOUT = 2;
    
    /**
     * A flag indicates that a new conversation started by robot.
     */
    public static final int OPEN_MODE_ROBOT = 3;
    
    
    
    /**
     * Send plain text message
     * @param message
     */
    public void send(String message) throws RobotException;
    /**
     * Send RobotMessage
     * @param message {@link com.incesoft.botplatform.sdk.RobotMessage}
     */
    public void send(RobotMessage message) throws RobotException;
    
    /**
     * Send nudge 
     */
    public void sendNudge() throws RobotException;
    
    /**
     * Show the content of network resource in MSN Activity window 
     * @param data network resource to be shown (generally are webpages or pictures)
     */
    public void sendActivity(String data) throws RobotException;
    
    /**
     * Show the content of network resource in MSN Activity window 
     * @param data network resource to be shown (generally are webpages or pictures)
     * @param friendlyName friendly name which represents the network resource in the activity invitation 
     */
    public void sendActivity(String data, String friendlyName) throws RobotException;
    

	/**
	 * Send activity data to the MSN Activity window.  
	 * @param appid id of the MSN Activity
	 * @param appname name of the MSN Activity
	 * @param data network resource to be shown (generally are webpages or pictures)
	 * @throws RobotException
	 */
	public void sendActivity(int appid, String appname, String data) throws RobotException;
    
	/**
	 * Check whether the activity window opened.
	 */
	public boolean isActivityOpened();
	
    /**
     * Send a typing infomation which will be presented on the bottom of the conversation panel
     */
    public void sendTyping() throws RobotException;
    
    /**
     * Create a message
     * @return RobotMessage {@link com.incesoft.botplatform.sdk.RobotMessage}
     */
    public RobotMessage createMessage();
    
    /**
     * Get a user's reference in session
     * @return RobotUser {@link com.incesoft.botplatform.sdk.RobotUser}
     */
    public RobotUser getUser();
    
    /**
     * Get a user's reference in session
	 * @param userid user id
	 * @return RobotUser {@link com.incesoft.botplatform.sdk.RobotUser}
	 */
	public RobotUser getUser(String userid);
	
	/**
	 * Get a users' collection in session
	 * @return Collection
	 */
	public Collection<RobotUser> getUsers();
    
    /**
     * Get the robot account(MSN Passport) associated with this session
     * @return
     */
    public String getRobot();
    
    /**
     * Get the open mode of session
     * @return openmode. 0 - Unknown; 1 - Indicates that user opens a conversation window; 2 - Indicates that session timeout and then reconnects  
     */
    public int getOpenMode();
    
    /**
     * Check whether the session is closed. If session is closed, you cannot send any messages.
     */
    public boolean isClosed();
    
    /**
     * Close this session.
     */
    public void close() throws RobotException;
    
    /**
     * Binds an object to this session, using the name specified. If an object of the same name is already bound to the session, the object is replaced.
     * @param name the name to which the object is bound; cannot be null
     * @param value the object to be bound 
     */
    public void setAttribute(String name, Object value);
    
    /**
     * Returns the object bound with the specified name in this session, or null if no object is bound under the name.
     * @param name a string specifying the name of the object
     * @return the object with the specified name 
     */
    public Object getAttribute(String name);
    
    /**
     * Removes the object bound with the specified name from this session. If the session does not have an object bound with the specified name, this method does nothing.
     * @param name the name of the object to remove from this session 
     * @return the object with the specified name
     */
    public Object removeAttribute(String name);
    
    
    /**
     * Returns an Iterator of String objects containing the names of all the objects bound to this session
     * @return an Iterator of String objects specifying the names of all the objects bound to this session 
     */
    public Iterator<String> getAttributeNames();
	
	/**
	 * Invite a user into the current conversation.
	 * @param user
	 * @throws RobotException
	 */
	public void inviteUser(String user) throws RobotException;
	
	/**
	 * Send a file
	 * @param uri unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
	 */
    public void sendFile(String uri) throws RobotException;
    
    /**
	 * Send a file
	 * @param uri unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
	 * @param friendlyName friendly name which represents the file in the file transfer invitation
	 */
    public void sendFile(String uri, String friendlyName) throws RobotException;
    
    /**
     * Accept a file transfer request, and a url should be specified for saving the file data.
     * @param transferId id of the current file transfer.
     * @param saveUrl a url to receive a HTTP POST request to get the data of the file.
     * @throws RobotException
     */
    public void sendFileAcceptance(String transferId, String saveUrl) throws RobotException ;
    
    /**
     * Reject a file transfer request. 
     * @param transferId id of the current file transfer.
     * @throws RobotException
     */
    public void sendFileRejection(String transferId)  throws RobotException ;
    
    /**
     * Cancel a file transfer.
     * @param transferId
     * @throws RobotException
     */
    public void sendFileCancellation(String transferId)  throws RobotException ;
    
    /**
     * Send a ink message
     * @param inkData data of the ink message.
     * @throws RobotException
     */
    public void sendInk(byte[] inkData) throws RobotException;
    
    /**
     * Send a msn default wink
     * @param uri unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
     */
    public void sendWink(String uri) throws RobotException;
    
    /**
     * Send a customized wink
     * @param uri unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
     * @param stamp base64 encoded signature of the file.It is a S/MIME signature of the Base64 encoded hash of the Content cabinet file and is signed by the MSN Content Authority. 
     */
    public void sendWink(String uri, String stamp) throws RobotException;
    
    /**
     * Send a voice clip
     * @param uri unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
     */
    public void sendVoiceclip(String uri) throws RobotException;
    
    /**
     * Send webcam
     * 
     * Once the connection is established, the client that connected to the other peer 
     *  should authenticate himself 
     *  with sending the message (no header, no nothing) : "recipientid=X&sessionid=Y\r\n\r\n"
     *  
     * The other peer should then send (if correctly authenticated) the message "connected\r\n\r\n" 
     * 
     * The connecting peer should also send "connected\r\n\r\n"
     * 
     * From this point the connection is done and The connecting peer is authenticated
     * 
     * Subsequently, the server may send the encoded data of the webcam in mimic format
     * 
     * @param serverIP webcam resource server IP address
     * @param serverPort webcam resource server port
     * @param recipientid the authentication info, should be between 100 and 199
     * @param sessionid the authentication info, should be between 9000 and 9999
     */
    public void sendWebcam(String serverIP, int serverPort, int recipientid, int sessionid) throws RobotException;
    
    
}
