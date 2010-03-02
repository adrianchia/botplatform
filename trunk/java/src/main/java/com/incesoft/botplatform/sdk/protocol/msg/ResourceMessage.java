package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

public class ResourceMessage implements Serializable{

	private static final long serialVersionUID = -6227137699187743115L;
	private String location;
	private String name;
	private String stamp;
	
	public String getLocation() {
		return location;
	}
	public void setLocation(String location) {
		this.location = location;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getStamp() {
		return stamp;
	}
	public void setStamp(String stamp) {
		this.stamp = stamp;
	}
}
