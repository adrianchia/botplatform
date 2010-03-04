package com.incesoft.botplatform.sdk;

import java.util.List;

public class RobotAdapter implements RobotHandler {

	public void activityAccepted(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void activityClosed(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void activityLoaded(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void activityReceived(RobotSession session, String data) {
		// TODO Auto-generated method stub

	}

	public void activityRejected(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void colorSchemeUpdated(String robot, String user, int colorScheme) {
		// TODO Auto-generated method stub

	}

	public void contactListReceived(String robot, List<RobotUser> contactList) {
		// TODO Auto-generated method stub

	}

	public void displayPictureUpdated(String robot, String user,
			RobotResource resource) {
		// TODO Auto-generated method stub

	}

	public void exceptionCaught(RobotSession session, Throwable cause) {
		// TODO Auto-generated method stub

	}

	public void fileAccepted(RobotSession session,
			RobotFileDescriptor invitation) {
		// TODO Auto-generated method stub

	}

	public void fileInvited(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		// TODO Auto-generated method stub

	}

	public void fileRejected(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		// TODO Auto-generated method stub

	}

	public void fileTransferCancelled(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		// TODO Auto-generated method stub

	}

	public void fileTransferEnded(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		// TODO Auto-generated method stub

	}

	public void fileTransferError(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		// TODO Auto-generated method stub

	}

	public void inkReceived(RobotSession session, byte[] ink) {
		// TODO Auto-generated method stub

	}

	public void messageReceived(RobotSession session, RobotMessage message) {
		// TODO Auto-generated method stub

	}

	public void nudgeReceived(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void personalMessageUpdated(String robot, String user,
			String personalMessage) {
		// TODO Auto-generated method stub

	}

	public void sceneUpdated(String robot, String user, RobotResource resource) {
		// TODO Auto-generated method stub

	}

	public void sessionClosed(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void sessionOpened(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void typingReceived(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void userAdd(String robot, String user) {
		// TODO Auto-generated method stub

	}

	public void userJoined(RobotSession session, RobotUser user) {
		// TODO Auto-generated method stub

	}

	public void userLeft(RobotSession session, RobotUser user) {
		// TODO Auto-generated method stub

	}

	public void userRemove(String robot, String user) {
		// TODO Auto-generated method stub

	}

	public void userUpdated(String robot, RobotUser user) {
		// TODO Auto-generated method stub

	}

	public void voiceclipReceived(RobotSession session, RobotResource resource) {
		// TODO Auto-generated method stub

	}

	public void webcamAccepted(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void webcamRejected(RobotSession session) {
		// TODO Auto-generated method stub

	}

	public void winkReceived(RobotSession session, RobotResource resource) {
		// TODO Auto-generated method stub

	}

}
