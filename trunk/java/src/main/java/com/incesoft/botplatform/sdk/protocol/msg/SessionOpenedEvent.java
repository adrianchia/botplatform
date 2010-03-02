package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

public class SessionOpenedEvent implements Serializable {

	private static final long serialVersionUID = 2988856824670240119L;

	private int mode;
	private User user;

	public int getMode() {
		return mode;
	}

	public void setMode(int mode) {
		this.mode = mode;
	}

	public User getUser() {
		return user;
	}

	public void setUser(User user) {
		this.user = user;
	}
}
