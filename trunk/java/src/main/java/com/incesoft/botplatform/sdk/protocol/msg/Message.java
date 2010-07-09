package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;
import java.util.HashMap;

import com.incesoft.botplatform.sdk.protocol.json.JsonUtil;

public class Message implements Serializable {

	private static final long serialVersionUID = 2829085563118195613L;

	public static final String LOGIN = "login";
	public static final String REDIRECT = "redirect";
	public static final String LOGINRESP = "loginresp";
	public static final String UPDATEROBOT = "updaterobot";
	public static final String USERUPDATED = "userupdated";
	public static final String PSMUPDATED = "psmupdated";
	public static final String USERADDED = "useradded";
	public static final String USERREMOVED = "userremoved";
	public static final String PUSH = "push";
	public static final String GETUSERLIST = "getuserlist";
	public static final String USERLIST = "userlist";
	public static final String GETRESOURCE = "getresource";
	public static final String RESOURCE = "resource";
	public static final String SESSIONOPENED = "sessionopened";
	public static final String SESSIONCLOSED = "sessionclosed";
	public static final String JOIN = "join";
	public static final String PART = "part";
	public static final String INVITE = "invite";
	public static final String CREATESESSION = "createsession";
	public static final String CLOSESESSION = "closesession";
	public static final String MSG = "msg";
	public static final String NUDGE = "nudge";
	public static final String APPMSG = "appmsg";
	public static final String APPEVENT = "appevent";
	public static final String TYPING = "typing";
	public static final String FILE = "file";
	public static final String FILECMD = "filecmd";
	public static final String FILEEVENT = "fileevent";
	public static final String BACKGROUND = "background";
	public static final String BACKGROUNDEVENT = "backgroundevent";
	public static final String BACKGROUNDINVITE = "backgroundinvite";
	public static final String INKMSG = "inkmsg";
	public static final String INK = "ink";
	public static final String WINK = "wink";
	public static final String WINKEVENT = "winkevent";
	public static final String VOICECLIP = "voiceclip";
	public static final String VOICECLIPEVENT = "voiceclipevent";
	public static final String WEBCAM = "webcam";
	public static final String WEBCAMEVENT = "webcamevent";
	public static final String ERROR = "error";
	public static final String DPUPDATED = "dpupdated";
	public static final String SCENEUPDATED = "sceneupdated";
	public static final String COLORUPDATED = "colorupdated";
	public static final String ADDUSER = "adduser";

	private String robotId;
	private String userId;
	private String sessionId;
	private String type;
	private Object body;

	public Message() {

	}

	public Message(String type, Object body) {
		this.type = type;
		this.body = body;
	}

	public Message(String robotId, String userId, String sessionId,
			String type, Object body) {
		this.robotId = robotId;
		this.userId = userId;
		this.sessionId = sessionId;
		this.type = type;
		this.body = body;
	}

	public String getRobotId() {
		return robotId;
	}

	public void setRobotId(String robotId) {
		this.robotId = robotId;
	}

	public String getUserId() {
		return userId;
	}

	public void setUserId(String userId) {
		this.userId = userId;
	}

	public String getSessionId() {
		return sessionId;
	}

	public void setSessionId(String sessionId) {
		this.sessionId = sessionId;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public Object getBody() {
		return body;
	}

	public void setBody(Object body) {
		this.body = body;
	}

	@SuppressWarnings("unchecked")
	public static Class mapBody(String type) {
		return BODY_MAP.get(type);
	}

	@SuppressWarnings("unchecked")
	public static final HashMap<String, Class> BODY_MAP = new HashMap<String, Class>();

	static {
		BODY_MAP.put(LOGIN, LoginRequest.class);
		BODY_MAP.put(REDIRECT, String[].class);
		BODY_MAP.put(LOGINRESP, LoginResponse.class);
		BODY_MAP.put(UPDATEROBOT, UpdateRobotRequest.class);
		BODY_MAP.put(USERUPDATED, User.class);
		BODY_MAP.put(PSMUPDATED, String.class);
		BODY_MAP.put(USERADDED, null);
		BODY_MAP.put(USERREMOVED, null);
		BODY_MAP.put(GETUSERLIST, null);
		BODY_MAP.put(USERLIST, User[].class);
		BODY_MAP.put(PUSH, TextMessage.class);
		BODY_MAP.put(GETRESOURCE, ResourceInfo.class);

		BODY_MAP.put(SESSIONOPENED, SessionOpenedEvent.class);
		BODY_MAP.put(SESSIONCLOSED, null);
		BODY_MAP.put(JOIN, User.class);
		BODY_MAP.put(PART, null);
		BODY_MAP.put(INVITE, String.class);
		BODY_MAP.put(CREATESESSION, null);
		BODY_MAP.put(CLOSESESSION, null);
		BODY_MAP.put(MSG, TextMessage.class);
		BODY_MAP.put(NUDGE, null);
		BODY_MAP.put(APPMSG, AppMessage.class);
		BODY_MAP.put(APPEVENT, String.class);
		BODY_MAP.put(TYPING, null);
		BODY_MAP.put(FILE, ResourceMessage.class);
		BODY_MAP.put(FILECMD, FileTransCommand.class);
		BODY_MAP.put(FILEEVENT, FileTransEvent.class);
		BODY_MAP.put(BACKGROUND, ResourceMessage.class);
		BODY_MAP.put(BACKGROUNDEVENT, String.class);
		BODY_MAP.put(BACKGROUNDINVITE, FileTransEvent.class);
		BODY_MAP.put(INKMSG, String.class);
		BODY_MAP.put(INK, ResourceMessage.class);
		BODY_MAP.put(WINK, ResourceMessage.class);
		BODY_MAP.put(WINKEVENT, ResourceInfo.class);
		BODY_MAP.put(VOICECLIP, ResourceMessage.class);
		BODY_MAP.put(VOICECLIPEVENT, ResourceInfo.class);
		BODY_MAP.put(WEBCAM, WebcamRequest.class);
		BODY_MAP.put(WEBCAMEVENT, String.class);
		BODY_MAP.put(ERROR, ErrorMessage.class);
		BODY_MAP.put(DPUPDATED, ResourceInfo.class);
		BODY_MAP.put(SCENEUPDATED, ResourceInfo.class);
		BODY_MAP.put(COLORUPDATED, Integer.class);
		BODY_MAP.put(RESOURCE, ResourceInfo.class);
		BODY_MAP.put(ADDUSER, String.class);
	};

	public String toString() {
		return JsonUtil.toJson(this);
	}
}
