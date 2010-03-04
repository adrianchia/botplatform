package com.incesoft.botplatform.sdk.support;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map;

import sun.misc.BASE64Encoder;

import com.incesoft.botplatform.sdk.RobotException;
import com.incesoft.botplatform.sdk.RobotMessage;
import com.incesoft.botplatform.sdk.RobotSession;
import com.incesoft.botplatform.sdk.RobotUser;
import com.incesoft.botplatform.sdk.protocol.msg.AppMessage;
import com.incesoft.botplatform.sdk.protocol.msg.FileTransCommand;
import com.incesoft.botplatform.sdk.protocol.msg.Message;
import com.incesoft.botplatform.sdk.protocol.msg.P2PEvent;
import com.incesoft.botplatform.sdk.protocol.msg.ResourceMessage;
import com.incesoft.botplatform.sdk.protocol.msg.WebcamRequest;

/**
 * @author LiBo
 */
public class DefaultRobotSession implements RobotSession {

	protected String sessionID;
	protected DefaultRobotServer server;
	private LinkedHashMap<String, RobotUser> users = new LinkedHashMap<String, RobotUser>();
	private RobotUser firstUser;
	private String robot;
	private int openMode = OPEN_MODE_UNKONW;
	private boolean closed = false;
	private Map<String, Object> attributes = new HashMap<String, Object>();
	private boolean activityOpened = false;

	public DefaultRobotSession(DefaultRobotServer server) {
		this.server = server;
	}

	public DefaultRobotSession(DefaultRobotServer server, String robotId,
			RobotUser user, String sessionID) {
		this.server = server;
		this.robot = robotId;
		this.firstUser = user;
		this.sessionID = sessionID;
		this.addUser(user);
	}

	public void send(String message) throws RobotException {
		RobotMessage msg = createMessage();
		msg.setString(message);
		send(msg);
	}

	public RobotMessage createMessage() {
		return new DefaultRobotMessage();
	}

	public RobotUser getUser() {
		if (firstUser != null)
			return firstUser;
		synchronized (users) {
			Iterator<RobotUser> i = users.values().iterator();
			if (i.hasNext())
				return (RobotUser) i.next();
		}
		return null;
	}

	public void addUser(RobotUser user) {
		this.users.put(user.getID(), user);
	}

	public RobotUser removeUser(String user) {
		return (RobotUser) this.users.remove(user);
	}

	public int getUserCount() {
		return users.size();
	}

	public RobotUser getUser(String userid) {
		return (RobotUser) users.get(userid);
	}

	public Collection<RobotUser> getUsers() {
		return Collections.unmodifiableCollection(users.values());
	}

	public boolean isClosed() {
		return closed;
	}

	public void setClosed(boolean closed) {
		this.closed = closed;
	}

	public int getOpenMode() {
		return openMode;
	}

	public void setOpenMode(int openMode) {
		this.openMode = openMode;
	}

	public String getRobot() {
		return robot;
	}

	public void setRobot(String robot) {
		this.robot = robot;
	}

	public void setAttribute(String name, Object value) {
		attributes.put(name, value);
	}

	public Object getAttribute(String name) {
		return attributes.get(name);
	}

	public Object removeAttribute(String name) {
		return attributes.remove(name);
	}

	public Iterator<String> getAttributeNames() {
		return attributes.keySet().iterator();
	}

	public boolean isActivityOpened() {
		return activityOpened;
	}

	public void setActivityOpened(boolean activityOpened) {
		this.activityOpened = activityOpened;
	}

	public void send(RobotMessage message) throws RobotException {
		DefaultRobotMessage msg = (DefaultRobotMessage) message;
		sendMessage(Message.MSG, msg.getProtocolMessage());

	}

	public void sendNudge() throws RobotException {
		sendMessage(Message.NUDGE, null);
	}

	public void sendActivity(String data) throws RobotException {
		sendActivity(data, null);
	}

	public void sendActivity(String data, String friendlyName)
			throws RobotException {
		AppMessage msg = new AppMessage();
		msg.setData(data);
		if (friendlyName != null)
			msg.setName(friendlyName);
		sendMessage(Message.APPMSG, msg);
	}

	public void sendTyping() throws RobotException {
		sendMessage(Message.TYPING, null);
	}

	public void sendFile(String uri) throws RobotException {
		sendFile(uri, null);
	}

	public void sendFile(String uri, String friendlyName) throws RobotException {
		ResourceMessage msg = new ResourceMessage();
		msg.setLocation(uri);
		if (friendlyName != null)
			msg.setName(friendlyName);
		sendMessage(Message.FILE, msg);
	}

	public void sendFileAcceptance(String transferId,String saveUrl)  throws RobotException {
		FileTransCommand cmd = new FileTransCommand();
		cmd.setCmd(P2PEvent.ACCEPT);
		cmd.setTransferId(transferId);
		cmd.setSaveUrl(saveUrl);
		sendMessage(Message.FILECMD, cmd);
	}
    
    public void sendFileRejection(String transferId) throws RobotException {
    	FileTransCommand cmd = new FileTransCommand();
		cmd.setCmd(P2PEvent.REJECT);
		cmd.setTransferId(transferId);
		sendMessage(Message.FILECMD, cmd);
    }
    public void sendFileCancellation(String transferId) throws RobotException {
    	FileTransCommand cmd = new FileTransCommand();
		cmd.setCmd(P2PEvent.CANCEL);
		cmd.setTransferId(transferId);
		sendMessage(Message.FILECMD, cmd);
    }
    

	public void sendInk(String uri) throws RobotException {
		ResourceMessage msg = new ResourceMessage();
		msg.setLocation(uri);
		sendMessage(Message.INK, msg);
	}

	public void sendInk(byte[] inkData) throws RobotException {
//		if (winkData.length == 0
//				|| winkData.length > InkMessage.WINK_DATA_LIMIT)
//			throw new RobotException(
//					"Ink data is empty or exceeds the 1024 bytes limit");
		String ink = new BASE64Encoder().encode(inkData);
		sendMessage(Message.INKMSG, ink);
	}

	public void sendWink(String uri) throws RobotException {
		sendWink(uri, null);
	}

	public void sendWink(String uri, String stamp) throws RobotException {
		ResourceMessage msg = new ResourceMessage();
		msg.setLocation(uri);
		if (stamp != null)
			msg.setStamp(stamp);
		sendMessage(Message.WINK, msg);
	}

	public void sendVoiceclip(String uri) throws RobotException {
		ResourceMessage msg = new ResourceMessage();
		if (uri != null) {
			msg.setLocation(uri);
			sendMessage(Message.VOICECLIP, msg);
		}
	}

	public void sendWebcam(String serverIP, int serverPort, int recipientid,
			int sessionid) throws RobotException {
		try {
			InetAddress.getByName(serverIP);
		} catch (UnknownHostException e) {
			throw new RobotException(e);
		}
		if (serverPort > 65535 || serverPort < 0)
			throw new RobotException("invalid server port.");
		if (recipientid > 199 || recipientid < 100)
			throw new RobotException(
					"recipientid should be between 100 and 199.");
		if (sessionid > 9999 || sessionid < 9000)
			throw new RobotException(
					"sessionid should be between 9000 and 9999.");
		WebcamRequest msg = new WebcamRequest();
		msg.setHost(serverIP);
		msg.setPort(serverPort);
		msg.setRid(recipientid);
		msg.setWid(sessionid);
		sendMessage(Message.WEBCAM, msg);
	}

	public void inviteUser(String user) throws RobotException {
		sendMessage(Message.INVITE, user);
	}

	public void sendActivity(int appid, String appname, String data)
			throws RobotException {
		AppMessage msg = new AppMessage();
		msg.setId(String.valueOf(appid));
		msg.setName(appname);
		msg.setData(data);
		sendMessage(Message.APPMSG, msg);
	}

	public void close() throws RobotException {
		sendMessage(Message.CLOSESESSION, null);
//		try {
//			this.setClosed(true);
//			server.process_sessionclosed(robot, getUser().getID(), sessionID,
//					null);
//		} catch (Exception e) {
//			throw new RobotException(e.getMessage(), e);
//		}
	}

	protected void sendMessage(String type, Object msg) throws RobotException {
		if (isClosed())
			throw new RobotException("session is closed.");
		try {
			server.sendMessage(new Message(robot, getUser().getID(), sessionID,
					type, msg));
		} catch (Exception e) {
			throw new RobotException(e.getMessage(), e);
		}
	}

	public String getSessionID() {
		return sessionID;
	}

	public void setSessionID(String sessionID) {
		this.sessionID = sessionID;
	}
}
