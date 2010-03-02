package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

public class ErrorMessage implements Serializable {

	private static final long serialVersionUID = -1830787806501724762L;

	private int code;
	private String message;

	public int getCode() {
		return code;
	}

	public void setCode(int code) {
		this.code = code;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}
}
