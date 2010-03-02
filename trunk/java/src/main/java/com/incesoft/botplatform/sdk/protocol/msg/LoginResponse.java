package com.incesoft.botplatform.sdk.protocol.msg;

public class LoginResponse {
	
	public static final int STATUS_OK = 1;
	public static final int STATUS_TOKEN_REQUIRED = 2;
	public static final int STATUS_FAILED = 3;
	
	private int status;
	private String connId;
	private String challenge;
	private String algorithm;
	
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public String getConnId() {
		return connId;
	}
	public void setConnId(String connId) {
		this.connId = connId;
	}
	public String getChallenge() {
		return challenge;
	}
	public void setChallenge(String challenge) {
		this.challenge = challenge;
	}
	public String getAlgorithm() {
		return algorithm;
	}
	public void setAlgorithm(String algorithm) {
		this.algorithm = algorithm;
	}
}
