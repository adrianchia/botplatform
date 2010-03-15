package com.incesoft.botplatform.sdk;

import java.util.List;

public class RobotAdapter implements RobotHandler {

	public void activityAccepted(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void activityClosed(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void activityLoaded(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void activityReceived(RobotSession session, String data) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void activityRejected(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void colorSchemeUpdated(String robot, String user, int colorScheme) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void contactListReceived(String robot, List<RobotUser> contactList) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void displayPictureUpdated(String robot, String user,
			RobotResource resource) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void exceptionCaught(RobotSession session, Throwable cause) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void fileAccepted(RobotSession session,
			RobotFileDescriptor invitation) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void fileInvited(RobotSession session,
			RobotFileDescriptor fileDescriptor) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void fileRejected(RobotSession session,
			RobotFileDescriptor fileDescriptor) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void fileTransferCancelled(RobotSession session,
			RobotFileDescriptor fileDescriptor) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void fileTransferEnded(RobotSession session,
			RobotFileDescriptor fileDescriptor) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void fileTransferError(RobotSession session,
			RobotFileDescriptor fileDescriptor) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void inkReceived(RobotSession session, byte[] ink) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void messageReceived(RobotSession session, RobotMessage message) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void nudgeReceived(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void personalMessageUpdated(String robot, String user,
			String personalMessage) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void sceneUpdated(String robot, String user, RobotResource resource) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void sessionClosed(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void sessionOpened(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void typingReceived(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void userAdd(String robot, String user) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void userJoined(RobotSession session, RobotUser user) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void userLeft(RobotSession session, RobotUser user) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void userRemove(String robot, String user) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void userUpdated(String robot, RobotUser user) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void voiceclipReceived(RobotSession session, RobotResource resource) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void webcamAccepted(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void webcamRejected(RobotSession session) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void winkReceived(RobotSession session, RobotResource resource) throws RobotException {
		// TODO Auto-generated method stub

	}

	public void fileReceived(String robot, String user, RobotFileDescriptor fileDescriptor, String saveUrl) {
		// TODO Auto-generated method stub
		
	}

	public void resourceReceived(String robot, String user, RobotResource resource, String saveUrl) {
		// TODO Auto-generated method stub
		
	}

}
