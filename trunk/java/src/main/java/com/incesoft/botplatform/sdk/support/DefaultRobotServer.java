package com.incesoft.botplatform.sdk.support;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import org.apache.commons.codec.digest.DigestUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import sun.misc.BASE64Decoder;

import com.incesoft.botplatform.sdk.RobotConnectionListener;
import com.incesoft.botplatform.sdk.RobotException;
import com.incesoft.botplatform.sdk.RobotHandler;
import com.incesoft.botplatform.sdk.RobotResource;
import com.incesoft.botplatform.sdk.RobotServer;
import com.incesoft.botplatform.sdk.RobotSession;
import com.incesoft.botplatform.sdk.RobotUser;
import com.incesoft.botplatform.sdk.protocol.msg.AppMessage;
import com.incesoft.botplatform.sdk.protocol.msg.ErrorMessage;
import com.incesoft.botplatform.sdk.protocol.msg.FileTransEvent;
import com.incesoft.botplatform.sdk.protocol.msg.LoginRequest;
import com.incesoft.botplatform.sdk.protocol.msg.LoginResponse;
import com.incesoft.botplatform.sdk.protocol.msg.Message;
import com.incesoft.botplatform.sdk.protocol.msg.P2PEvent;
import com.incesoft.botplatform.sdk.protocol.msg.ResourceInfo;
import com.incesoft.botplatform.sdk.protocol.msg.SessionOpenedEvent;
import com.incesoft.botplatform.sdk.protocol.msg.TextMessage;
import com.incesoft.botplatform.sdk.protocol.msg.UpdateRobotRequest;
import com.incesoft.botplatform.sdk.protocol.msg.User;

/**
 * @author LiBo
 */
public class DefaultRobotServer extends RobotConnection implements RobotServer {

	protected static Log log = LogFactory.getLog(DefaultRobotServer.class);

	public static final int KEEP_ALIVE_TIMEOUT = 60000;
	public static final long DEFAULT_LOGIN_TIMEOUT = 60000;
	public static final String VERSION = "java-3.0";

	protected String host;
	protected int port;
	protected boolean loggedIn;
	protected boolean reconnectedSupport;

	protected ScheduledExecutorService scheduledExecutor = null;
	@SuppressWarnings("unchecked")
	protected ScheduledFuture scheduledFuture = null;
	protected int lifeStatus = 3;

	protected List<RobotConnectionListener> connectionListeners = null;
	protected RobotHandler handler = null;
	protected HashMap<String, Method> processors = new HashMap<String, Method>();
	protected ConcurrentHashMap<String, DefaultRobotSession> sessions = new ConcurrentHashMap<String, DefaultRobotSession>();

	protected LoginStatus loginStatus = new LoginStatus();
	protected int loginState = 0;

	protected String SPID;
	protected String SPPWD;
	protected long timeout;

	public DefaultRobotServer() {
		Method[] methods = this.getClass().getMethods();
		for (Method method : methods) {
			String name = method.getName();
			if (name.startsWith("process_")) {
				processors.put(name.substring(8), method);
			}
		}
	}

	public void process_loginresp(String robotId, String userId,
			String sessionId, LoginResponse resp) throws Exception {
		if (resp.getStatus() == LoginResponse.STATUS_OK) {
			loggedIn = true;
			loginStatus.setStatus(LoginStatus.OK);
		} else if (resp.getStatus() == LoginResponse.STATUS_TOKEN_REQUIRED) {
			String data = resp.getChallenge() + getSPID() + resp.getConnId()
					+ getSPPWD();
			String token = DigestUtils.md5Hex(data);
			init(token);
		} else {
			loggedIn = false;
			loginStatus.setStatus(LoginStatus.FAILED);
		}
	}

	public void process_redirect(String robotId, String userId,
			String sessionId, String[] addrlist) throws Exception {
		for (String addr : addrlist) {
			int i = addr.indexOf(':');
			host = addr.substring(0, i);
			port = Integer.parseInt(addr.substring(i + 1));
			loginStatus.setStatus(LoginStatus.REDIRECT);
			break; // select the first one
		}
	}

	public void process_userupdated(String robotId, String userId,
			String sessionId, RobotUser user) throws Exception {
		if (handler != null)
			handler.userUpdated(robotId, user);
	}

	public void process_useradded(String robotId, String userId,
			String sessionId, Object obj) throws Exception {
		if (handler != null)
			handler.userAdd(robotId, userId);
	}

	public void process_userremoved(String robotId, String userId,
			String sessionId, Object obj) throws Exception {
		if (handler != null)
			handler.userRemove(robotId, userId);
	}

	public void process_userremoved(String robotId, String userId,
			String sessionId, String psm) throws Exception {
		if (handler != null)
			handler.personalMessageUpdated(robotId, userId, psm);
	}

	public void process_userlist(String robotId, String userId,
			String sessionId, User[] userlist) throws Exception {
		if (handler != null && userlist != null) {
			List<RobotUser> rulist = new ArrayList<RobotUser>();
			for (User u : userlist) {
				rulist.add(u);
			}
			handler.contactListReceived(robotId, rulist);
		}
	}

	public void process_sessionopened(String robotId, String userId,
			String sessionId, SessionOpenedEvent event) throws Exception {
		DefaultRobotSession session = new DefaultRobotSession(
				DefaultRobotServer.this);
		session.setRobot(robotId);
		session.addUser(event.getUser());
		session.setSessionID(sessionId);
		session.setOpenMode(event.getMode());
		sessions.put(sessionId, session);
		if (handler != null)
			handler.sessionOpened(session);
	}

	public void process_sessionclosed(String robotId, String userId,
			String sessionId, Object obj) throws Exception {
		DefaultRobotSession session = sessions.remove(sessionId);
		if (session != null) {
			session.setClosed(true);
			if (handler != null){
				handler.sessionClosed(session);
			}
		}
	}

	public void process_join(String robotId, String userId, String sessionId,
			User user) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			session.addUser(user);
			if (handler != null)
				handler.userJoined(session, user);
		}
	}

	public void process_part(String robotId, String userId, String sessionId,
			Object obj) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			session.removeUser(userId);
			if (handler != null)
				handler.userLeft(session, session.getUser(userId));
		}
	}

	public void process_msg(String robotId, String userId, String sessionId,
			TextMessage msg) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			if (handler != null)
				handler.messageReceived(session, new DefaultRobotMessage(msg));
		}
	}

	public void process_nudge(String robotId, String userId, String sessionId,
			Object obj) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			if (handler != null)
				handler.nudgeReceived(session);
		}
	}

	public void process_appmsg(String robotId, String userId, String sessionId,
			AppMessage app) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			if (handler != null)
				handler.activityReceived(session, app.getData());
		}
	}

	public void process_appevent(String robotId, String userId,
			String sessionId, String event) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			if (handler != null) {
				if (P2PEvent.ACCEPT.equals(event)){
					session.setActivityOpened(true);
					handler.activityAccepted(session);
				}
				else if (P2PEvent.REJECT.equals(event))
					handler.activityRejected(session);
				else if (P2PEvent.READY.equals(event))
					handler.activityLoaded(session);
				else if (P2PEvent.CLOSE.equals(event)){
					session.setActivityOpened(false);
					handler.activityClosed(session);
				}
			}
		}
	}

	public void process_fileevent(String robotId, String userId,
			String sessionId, FileTransEvent eventObj) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			if (handler != null) {
				String event = eventObj.getEvent();
				if (P2PEvent.ACCEPT.equals(event))
					handler.fileAccepted(session,eventObj);
				else if (P2PEvent.REJECT.equals(event))
					handler.fileRejected(session,eventObj);
				else if (P2PEvent.INVITE.equals(event)) 
					handler.fileInvited(session,eventObj);
				else if (P2PEvent.CLOSE.equals(event))
					handler.fileTransferEnded(session,eventObj);
				else if (P2PEvent.ERROR.equals(event))
					handler.fileTransferError(session,eventObj);
				else if (P2PEvent.CANCEL.equals(event))
					handler.fileTransferCancelled(session,eventObj);
				else if(P2PEvent.RECEIVE.equals(event))
					handler.fileReceived(robotId, userId, eventObj, eventObj.getSaveUrl());
			}
		}
	}

	public void process_fileinvite(String robotId, String userId,
			String sessionId, FileTransEvent invitation) throws RobotException {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null && handler != null) {
			handler.fileInvited(session, invitation);
		}
	}

	public void process_webcamevent(String robotId, String userId,
			String sessionId, String event) throws Exception {
		DefaultRobotSession session = sessions.get(sessionId);
		if (session != null) {
			if (handler != null) {
				if (P2PEvent.ACCEPT.equals(event))
					handler.webcamAccepted(session);
				else if (P2PEvent.REJECT.equals(event))
					handler.webcamRejected(session);
			}
		}
	}

	public void process_error(String robotId, String userId, String sessionId,
			ErrorMessage err) throws Exception {
		if (log.isErrorEnabled())
			log.error("process_error:\r\n\terrorCode: " + err.getCode()
					+ "\r\n\terrorMessage: " + err.getMessage());

		RobotException exception = new RobotException(err.getCode(), err
				.getMessage());
		loginStatus.setException(exception);
		if (handler != null) {
			RobotSession session = (RobotSession) sessions.get(sessionId);
			handler.exceptionCaught(session, exception);
		}
	}

	public void process_typing(String robotId, String userId, String sessionId,
			Object msgBoby) throws RobotException {
		DefaultRobotSession session = sessions.get(sessionId);
		if (handler != null && session != null)
			handler.typingReceived(session);
	}

	public void process_voiceclipevent(String robotId, String userId,
			String sessionId, ResourceInfo resource) throws RobotException {
		DefaultRobotSession session = sessions.get(sessionId);
		if (handler != null && session != null)
			handler.voiceclipReceived(session, resource);
	}

	public void process_winkevent(String robotId, String userId,
			String sessionId, ResourceInfo resource) throws RobotException {
		DefaultRobotSession session = sessions.get(sessionId);
		if (handler != null && session != null)
			handler.winkReceived(session, resource);
	}

	public void process_inkmsg(String robotId, String userId, String sessionId,
			String ink) throws RobotException {
		DefaultRobotSession session = sessions.get(sessionId);
		if (handler != null && session != null){
			byte[] inkData = null;
			try {
				inkData = new BASE64Decoder().decodeBuffer(ink);
			} catch (IOException e) {}
			handler.inkReceived(session, inkData);
		}
	}

	public void process_dpupdated(String robotId, String userId,
			String sessionId, ResourceInfo resource) throws RobotException {
		if (handler != null)
			handler.displayPictureUpdated(robotId, userId, resource);
	}

	public void process_sceneupdated(String robotId, String userId,
			String sessionId, ResourceInfo resource) throws RobotException {
		if (handler != null)
			handler.sceneUpdated(robotId, userId, resource);
	}

	public void process_colorupdated(String robotId, String userId,
			String sessionId, int colorScheme) throws RobotException {
		if (handler != null)
			handler.colorSchemeUpdated(robotId, userId, colorScheme);
	}

	public void process_psmupdated(String robotId, String userId,
			String sessionId, String psm) throws RobotException {
		if (handler != null)
			handler.personalMessageUpdated(robotId, userId, psm);
	}
	
	public void process_resource(String robotId, String userId,
			String sessionId, ResourceInfo resource) {
		if (handler != null)
			handler.resourceReceived(robotId, userId, resource, resource.getSaveUrl());
	}
	
	public void login(String spid, String sppwd) throws RobotException {
		login(spid, sppwd, DEFAULT_LOGIN_TIMEOUT);
	}

	public void login(String spid, String sppwd, long timeout)
			throws RobotException {

		this.SPID = spid;
		this.SPPWD = sppwd;
		this.timeout = timeout;

		loginImpl(true);
	}

	protected void loginImpl(boolean syn) throws RobotException {
		try {
			open(host, port);
			if (syn) {
				int status = loginStatus.getStatus(timeout);
				if (status == LoginStatus.OK) {
					if (isReconnectedSupport()) {
						synchronized (this) {
							if (scheduledFuture == null) {
								scheduledFuture = scheduledExecutor
										.scheduleAtFixedRate(
												new KeepAliveTask(),
												KEEP_ALIVE_TIMEOUT,
												KEEP_ALIVE_TIMEOUT,
												TimeUnit.MILLISECONDS);
							}
						}
					}
				} else {
					synchronized (this) {
						if (session != null) {
							session.close();
							session = null;
						}
					}
					if (status == LoginStatus.REDIRECT) {
						loginStatus = new LoginStatus();
						loginState++;
						loginImpl(true);
					} else if (status == LoginStatus.TIMEOUT) {
						throw new RobotException("login timeout.");
					} else {
						RobotException exception = loginStatus.getException();
						if (exception != null)
							throw exception;
						else
							throw new RobotException("login faild.");
					}
				}
			}
		} catch (Exception e) {
			throw new RobotException(e.getMessage(), e);
		} finally {
			loginStatus = new LoginStatus();
			loginState = 0;
		}
	}

	public synchronized void logout() {
		if (scheduledFuture != null) {
			scheduledFuture.cancel(true);
			scheduledFuture = null;
		}
		logoutImpl();

	}

	public void logoutImpl() {
		if (session != null)
			session.close();

	}

	protected void init(String token) throws Exception {

		LoginRequest req = new LoginRequest();
		req.setSpid(SPID);
		req.setVersion(VERSION);
		req.setToken(token);
		req.setState(loginState);
		sendMessage(new Message(Message.LOGIN, req));
	}

	public void assertAlive() {
		lifeStatus = 3;
	}

	public void setDisplayName(String displayName) throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (displayName == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setDisplayName(displayName);
		sendMessage(new Message(Message.UPDATEROBOT, req));
	}

	public void setDisplayName(String robotAccount, String displayName)
			throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (displayName == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setDisplayName(displayName);
		sendMessage(new Message(robotAccount, null, null, Message.UPDATEROBOT,
				req));
	}

	public void setPersonalMessage(String personalMessage)
			throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (personalMessage == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setPersonalMessage(personalMessage);
		sendMessage(new Message(Message.UPDATEROBOT, req));
	}

	public void setPersonalMessage(String robotAccount, String personalMessage)
			throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (personalMessage == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setPersonalMessage(personalMessage);
		sendMessage(new Message(robotAccount, null, null, Message.UPDATEROBOT,
				req));
	}

	public void setDisplayPicture(String displayPicture) throws RobotException {
		setDisplayPictureEx(displayPicture, null);
	}

	public void setDisplayPicture(String robotAccount, String displayPicture)
			throws RobotException {
		setDisplayPictureEx(robotAccount, displayPicture, null);
	}

	public void createSession(String robot, String user) throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (robot == null || user == null)
			throw new NullPointerException();
		sendMessage(new Message(robot, user, null, Message.CREATESESSION, null));
	}

	public void pushMessage(String robot, String user, String message)
			throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (robot == null || user == null || message == null)
			throw new NullPointerException();
		sendMessage(new Message(robot, user, null, Message.PUSH, message));
	}

	public void requestContactList(String robot) throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not loggedin.");
		if (robot == null)
			throw new NullPointerException();
		sendMessage(new Message(robot, null, null, Message.GETUSERLIST, null));
	}
	
	public void requestResource(String robot, String user, RobotResource resource, String saveUrl) throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not loggedin.");
		if (robot == null || user == null || resource == null)
			throw new NullPointerException();
		ResourceInfo info = new ResourceInfo();
		info.setDigest(resource.getDigest());
		info.setName(resource.getName());
		info.setSize(resource.getSize());
		info.setSaveUrl(saveUrl);
		sendMessage(new Message(robot, user, null, Message.GETRESOURCE, info));
	}

	@Override
	protected void processOpened() throws Exception {
		synchronized (this) {
			init(null);
			if (scheduledFuture == null)
				this.fireServerConnected();
			else
				this.fireServerReconnected();
		}
	}

	@Override
	protected void processClosed() throws Exception {
		synchronized (this) {
			this.setLoggedIn(false);
			this.fireServerDisconnected();
		}
	}

	@Override
	protected void processError(Throwable e) throws RobotException {
		if(log.isErrorEnabled())
			log.error("",e);
		if (handler != null) {
			handler.exceptionCaught(null, e);
		}
	}

	@Override
	protected void processMessage(Message msg) throws Exception {
		assertAlive();
		Method method = processors.get(msg.getType());
		if (method != null) {
			try {
				method.invoke(this, new Object[] { msg.getRobotId(),
						msg.getUserId(), msg.getSessionId(), msg.getBody() });
			} catch (InvocationTargetException e) {
				if (handler != null) 
					handler.exceptionCaught(null, e.getTargetException());
			}
		}
	}

	@Override
	protected void processSent() throws Exception {
		assertAlive();
	}

	private static class LoginStatus {
		public static int TIMEOUT = 0;
		public static int OK = 1;
		public static int REDIRECT = 2;
		public static int FAILED = 3;

		private int status = TIMEOUT;
		private RobotException exception = null;

		public synchronized int getStatus(long timeout) {
			try {
				this.wait(timeout);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			return status;
		}

		public synchronized void setStatus(int status) {
			this.status = status;
			this.notifyAll();
		}

		public RobotException getException() {
			return exception;
		}

		public synchronized void setException(RobotException e) {
			this.status = FAILED;
			this.exception = e;
			this.notifyAll();
		}
	}

	private class KeepAliveTask implements Runnable {

		public void run() {
			try {
				if (lifeStatus <= 0) {
					DefaultRobotServer.this.logoutImpl();
					DefaultRobotServer.this.loginImpl(true);
				} else {
					lifeStatus--;
					keepAlive();
				}
			} catch (Exception e) {
				if (log.isErrorEnabled())
					log.error(e.getMessage(), e);
			}
		}

	}

	public String getSPID() {
		return SPID;
	}

	public synchronized void setReconnectedSupport(boolean b) {
		this.reconnectedSupport = b;
		if (isLoggedIn()) {
			if (b) {
				if (scheduledFuture == null) {
					scheduledFuture = scheduledExecutor.scheduleAtFixedRate(
							new KeepAliveTask(), KEEP_ALIVE_TIMEOUT,
							KEEP_ALIVE_TIMEOUT, TimeUnit.MILLISECONDS);
				}
			} else {
				if (scheduledFuture != null) {
					scheduledFuture.cancel(true);
					scheduledFuture = null;
				}
			}
		}
	}

	public boolean isReconnectedSupport() {
		return reconnectedSupport;
	}

	public void addConnectionListener(RobotConnectionListener listener) {
		if (connectionListeners == null)
			connectionListeners = new ArrayList<RobotConnectionListener>();
		connectionListeners.add(listener);
	}

	public void removeConnectionistener(RobotConnectionListener listener) {
		if (connectionListeners != null)
			connectionListeners.remove(listener);
	}

	public void fireServerConnected() {
		if (connectionListeners != null) {
			for (RobotConnectionListener listener : connectionListeners) {
				listener.serverConnected(this);
			}
		}
	}

	public void fireServerReconnected() {
		if (connectionListeners != null) {
			for (RobotConnectionListener listener : connectionListeners) {
				listener.serverReconnected(this);
			}
		}
	}

	public void fireServerDisconnected() {
		if (connectionListeners != null) {
			for (RobotConnectionListener listener : connectionListeners) {
				listener.serverDisconnected(this);
			}
		}
	}

	public void setSPID(String spid) {
		SPID = spid;
	}

	public String getSPPWD() {
		return SPPWD;
	}

	public void setSPPWD(String sppwd) {
		SPPWD = sppwd;
	}

	public boolean isLoggedIn() {
		return loggedIn;
	}

	public void setLoggedIn(boolean loggedIn) {
		this.loggedIn = loggedIn;
	}

	public void setRobotHandler(RobotHandler handler) {
		this.handler = handler;
	}

	public RobotHandler getRobotHandler() {
		return handler;
	}

	public String getHost() {
		return host;
	}

	public void setHost(String host) {
		this.host = host;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public ScheduledExecutorService getScheduledExecutor() {
		return scheduledExecutor;
	}

	public void setScheduledExecutor(ScheduledExecutorService scheduledExecutor) {
		this.scheduledExecutor = scheduledExecutor;
	}

	public void setColorScheme(int colorScheme) throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setColorScheme(colorScheme);
		sendMessage(new Message(null, null, null, Message.UPDATEROBOT, req));
	}

	public void setColorScheme(String robot, int colorScheme)
			throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setColorScheme(colorScheme);
		sendMessage(new Message(null, null, null, Message.UPDATEROBOT, req));
	}

	public void setScene(String scene) throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (scene == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setScene(scene);
		sendMessage(new Message(null, null, null, Message.UPDATEROBOT, req));
	}

	public void setScene(String robotAccount, String scene)
			throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (scene == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setScene(scene);
		sendMessage(new Message(robotAccount, null, null, Message.UPDATEROBOT,
				req));
	}

	public void setDisplayPictureEx(String displayPicture, String deluxePicture)
			throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (displayPicture == null || deluxePicture == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setDisplayPicture(displayPicture);
		req.setLargePicture(deluxePicture);
		sendMessage(new Message(Message.UPDATEROBOT, req));
	}

	public void setDisplayPictureEx(String robotAccount, String displayPicture,
			String deluxePicture) throws RobotException {
		if (!this.isLoggedIn())
			throw new RobotException("robot server not logged in.");
		if (displayPicture == null || deluxePicture == null)
			throw new NullPointerException();
		UpdateRobotRequest req = new UpdateRobotRequest();
		req.setDisplayPicture(displayPicture);
		req.setLargePicture(deluxePicture);
		sendMessage(new Message(robotAccount, null, null, Message.UPDATEROBOT,
				req));
	}

}
