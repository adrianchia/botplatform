package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

public class WebcamRequest implements Serializable{
	
	private static final long serialVersionUID = 231941856123150698L;
	private String host;
	private int port;
	private int rid;
	private int wid;
	
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
	public int getRid() {
		return rid;
	}
	public void setRid(int rid) {
		this.rid = rid;
	}
	public int getWid() {
		return wid;
	}
	public void setWid(int wid) {
		this.wid = wid;
	}
}
