/*
 * Created on 2006-2-20
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk;

import java.util.List;

/**
 * Robot event handler
 * 
 * @author LiBo
 */
public interface RobotHandler {

	/**
	 * Occurs when robot receives a user's chat request (session setup), which
	 * is caused by a user opens a conversation window or session timeout and
	 * then reconnects.
	 */
	public void sessionOpened(RobotSession session);

	/**
	 * Occurs when user leaves a session (session closes), which is causedby a
	 * user closes a conversation window or session timeout.
	 */
	public void sessionClosed(RobotSession session);

	/**
	 * Occurs when robot receives a user's message
	 * 
	 * @param message
	 *            Received message
	 */
	public void messageReceived(RobotSession session, RobotMessage message);

	/**
	 * Occurs when robot receives a user's nudge
	 */
	public void nudgeReceived(RobotSession session);

	/**
	 * Occurs when user accepts an activity (P4) invitation
	 */
	public void activityAccepted(RobotSession session);

	/**
	 * Occurs when user rejects an activity (P4) invitation
	 */
	public void activityRejected(RobotSession session);

	/**
	 * Occurs when robot receives a user's activity-data( ms activity sdk ->
	 * Channel.SendData(...) ) ( ** Only available in enterprise version ** )
	 */
	public void activityReceived(RobotSession session, String data);

	/**
	 * Occurs when the activity associated with this session loaded. ( ** Only
	 * available in enterprise version ** )
	 */
	public void activityLoaded(RobotSession session);

	/**
	 * Occurs when the activity associated with this session is closed. ( **
	 * Only available in enterprise version ** )
	 */
	public void activityClosed(RobotSession session);

	/**
	 * ( ** Only available in enterprise version ** )
	 */
	public void userJoined(RobotSession session, RobotUser user);

	/**
	 * ( ** Only available in enterprise version ** )
	 */
	public void userLeft(RobotSession session, RobotUser user);

	/**
	 * Occurs when user adds the robot to his friend list ( ** Only available in
	 * enterprise version ** )
	 * 
	 * @param user
	 */
	public void userAdd(String robot, String user);

	/**
	 * Occurs when user removes the robot from his friend list
	 * 
	 * @param user
	 */
	public void userRemove(String robot, String user);

	/**
	 * Occurs when BOTPLATFORM server throws an exception associated with this
	 * session
	 */
	public void exceptionCaught(RobotSession session, Throwable cause);

	/**
	 * Occurs when user accepts a file transfer invitation ( ** Only available
	 * in enterprise version ** )
	 */
	public void fileAccepted(RobotSession session, RobotFileDescriptor invitation);

	/**
	 * Occurs when user rejects a file transfer invitation ( ** Only available
	 * in enterprise version ** )
	 */
	public void fileRejected(RobotSession session, RobotFileDescriptor fileDescriptor);

	/**
	 * Occurs when a file transfer ended ( ** Only available in enterprise
	 * version ** )
	 */
	public void fileTransferEnded(RobotSession session, RobotFileDescriptor fileDescriptor);

	/**
	 * Occurs when a file transfer cancelled ( ** Only available in enterprise
	 * version ** )
	 */
	public void fileTransferCancelled(RobotSession session, RobotFileDescriptor fileDescriptor);

	/**
	 * Occurs when a file transfer encounters an error ( ** Only available in
	 * enterprise version ** )
	 */
	public void fileTransferError(RobotSession session, RobotFileDescriptor fileDescriptor);

	/**
	 * Occurs when a file invite request is received ( ** Only available in
	 * enterprise version ** )
	 */
	public void fileInvited(RobotSession session, RobotFileDescriptor fileDescriptor);

	/**
	 * Occurs when user accepts a webcam invitation ( ** Only available in
	 * enterprise version ** )
	 */
	public void webcamAccepted(RobotSession session);

	/**
	 * Occurs when user rejects a webcam invitation ( ** Only available in
	 * enterprise version ** )
	 */
	public void webcamRejected(RobotSession session);

	/**
	 * Occurs when user changes nickname or status.
	 */
	public void userUpdated(String robot, RobotUser user);

	/**
	 * Occurs when user changes personal message.
	 */
	public void personalMessageUpdated(String robot, String user,
			String personalMessage);

	/**
	 * Occurs when robot receives ink message .
	 * 
	 * @param session
	 */
	public void inkReceived(RobotSession session,byte[] ink);

	/**
	 * Occurs when robot receives wink message .
	 * 
	 * @param session
	 * @param winkName -
	 *            display name of the wink
	 */
	public void winkReceived(RobotSession session, RobotResource resource);

	/**
	 * Occurs when robot receives voice clip .
	 * 
	 * @param session
	 */
	public void voiceclipReceived(RobotSession session,RobotResource resource);

	/**
	 * Occurs when robot receives user typing message .
	 * 
	 * @param session
	 */
	public void typingReceived(RobotSession session);

	/**
	 * Occurs when robot receives scene request .
	 * 
	 * @param session
	 */
	public void sceneUpdated(String robot, String user, RobotResource resource);

	/**
	 * Occurs when a user changes his/her display picture .
	 * 
	 * @param robot
	 * @param user
	 * @param uri -
	 *            the uri of the display picture
	 */
	public void displayPictureUpdated(String robot, String user, RobotResource resource);
	
	public void colorSchemeUpdated(String robot, String user, int colorScheme);
	
	/**
	 * Occurs when robot receives contact list .
	 * 
	 * @param robot
	 * @param contactList
	 */
	public void contactListReceived(String robot, List<RobotUser> contactList);

}
