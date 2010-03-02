package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

import com.incesoft.botplatform.sdk.RobotUser;

public class User implements RobotUser, Serializable {

	private static final long serialVersionUID = 3170070673004644854L;

	private String id;
	private String friendlyName;
	private String status;
	private long clientID;

	public String getID() {
		return id;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getFriendlyName() {
		return friendlyName;
	}

	public void setFriendlyName(String friendlyName) {
		this.friendlyName = friendlyName;
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	public long getClientID() {
		return clientID;
	}

	public void setClientID(long clientID) {
		this.clientID = clientID;
	}
}
