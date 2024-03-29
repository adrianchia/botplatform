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
	public void sessionOpened(RobotSession session) throws RobotException;

	/**
	 * Occurs when user leaves a session (session closes), which is caused by a
	 * user closes a conversation window or session timeout.
	 */
	public void sessionClosed(RobotSession session) throws RobotException;

	/**
	 * Occurs when robot receives a user's message
	 * 
	 * @param message Received message
	 */
	public void messageReceived(RobotSession session, RobotMessage message) throws RobotException;

	/**
	 * Occurs when robot receives a user's nudge
	 */
	public void nudgeReceived(RobotSession session) throws RobotException;

	/**
	 * Occurs when user accepts an activity (P4) invitation
	 */
	public void activityAccepted(RobotSession session) throws RobotException;

	/**
	 * Occurs when user rejects an activity (P4) invitation
	 */
	public void activityRejected(RobotSession session) throws RobotException;

	/**
	 * Occurs when robot receives a user's activity-data( ms activity sdk -> Channel.SendData(...) )
	 */
	public void activityReceived(RobotSession session, String data) throws RobotException;

	/**
	 * Occurs when the activity associated with this session loaded.
	 */
	public void activityLoaded(RobotSession session) throws RobotException;

	/**
	 * Occurs when the activity associated with this session is closed.
	 */
	public void activityClosed(RobotSession session) throws RobotException;

	/**
	 * Occurs when the a user joins the current conversation.
	 */
	public void userJoined(RobotSession session, RobotUser user) throws RobotException;

	/**
	 * Occurs when the a user leaves the current conversation.
	 */
	public void userLeft(RobotSession session, RobotUser user) throws RobotException;

	/**
	 * Occurs when user adds the robot to his friend list.
	 * 
	 * @param user
	 */
	public void userAdd(String robot, String user) throws RobotException;

	/**
	 * Occurs when user removes the robot from his friend list
	 * 
	 * @param user
	 */
	public void userRemove(String robot, String user) throws RobotException;

	/**
	 * Occurs when BOTPLATFORM server throws an exception associated with this session
	 */
	public void exceptionCaught(RobotSession session, Throwable cause) throws RobotException;

	/**
	 * Occurs when user accepts a file transfer invitation.
	 */
	public void fileAccepted(RobotSession session, RobotFileDescriptor invitation) throws RobotException;

	/**
	 * Occurs when user rejects a file transfer invitation.
	 */
	public void fileRejected(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;

	/**
	 * Occurs when a file transfer ended.
	 */
	public void fileTransferEnded(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;

	/**
	 * Occurs when a file transfer cancelled.
	 */
	public void fileTransferCancelled(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;

	/**
	 * Occurs when a file transfer encounters an error.
	 */
	public void fileTransferError(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;

	/**
	 * Occurs when a file has been saved to the saveUrl.
	 */
	public void fileReceived(String robot, String user, RobotFileDescriptor fileDescriptor, String saveUrl);
	
	/**
	 * Occurs when a file invite request is received.
	 */
	public void fileInvited(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;

	/**
	 * Occurs when user accepts a webcam invitation.
	 */
	public void webcamAccepted(RobotSession session) throws RobotException;

	/**
	 * Occurs when user rejects a webcam invitation.
	 */
	public void webcamRejected(RobotSession session) throws RobotException;

	/**
	 * Occurs when user changes nickname or status.
	 */
	public void userUpdated(String robot, RobotUser user) throws RobotException;

	/**
	 * Occurs when user changes personal message.
	 */
	public void personalMessageUpdated(String robot, String user, String personalMessage) throws RobotException;

	/**
	 * Occurs when robot receives ink message .
	 * 
	 * @param ink the data of the received ink message.
	 */
	public void inkReceived(RobotSession session,byte[] ink) throws RobotException;

	/**
	 * Occurs when robot receives wink message.
	 * 
	 * @param resource a descriptor of the received wink message (see RobotResource).
	 */
	public void winkReceived(RobotSession session, RobotResource resource) throws RobotException;

	/**
	 * Occurs when robot receives voice clip .
	 * 
	 * @param resource a descriptor of the received voice clip (see RobotResource).
	 */
	public void voiceclipReceived(RobotSession session,RobotResource resource) throws RobotException;

	/**
	 * Occurs user is inputting messages.
	 */
	public void typingReceived(RobotSession session) throws RobotException;

	/**
	 * Occurs when user changes scene.
	 * 
	 * @param resource a descriptor of the received scene (see RobotResource).
	 */
	public void sceneUpdated(String robot, String user, RobotResource resource) throws RobotException;

	/**
	 * Occurs when a user changes display picture .
	 * 
	 * @param robot robot account.
	 * @param user user who changes display picture.
	 * @param resource a descriptor of the display picture (see RobotResource).
	 */
	public void displayPictureUpdated(String robot, String user, RobotResource resource) throws RobotException;
	
	/**
	 * 
	 * @param robot robot account.
	 * @param user user who changes color scheme.
	 * @param colorScheme color
	 */
	public void colorSchemeUpdated(String robot, String user, int colorScheme) throws RobotException;
	
	/**
	 * Occurs when robot receives contact list.
	 * 
	 * @param robot robot account.
	 * @param contactList a list contains the account's friend list (see RobotUser).
	 */
	public void contactListReceived(String robot, List<RobotUser> contactList) throws RobotException;

	
	/**
	 * Occurs when a resource has been saved to the saveUrl. 
	 */
	public void resourceReceived(String robot, String user,RobotResource resource, String saveUrl);
}
