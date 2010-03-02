package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

import com.incesoft.botplatform.sdk.RobotResource;

public class ResourceInfo implements RobotResource, Serializable {
	
	private static final long serialVersionUID = -3606588117994378560L;
	
	private String digest;
	private String name;
	private int size;
	private String saveUrl;

	public String getDigest() {
		return digest;
	}

	public String getName() {
		return name;
	}

	public int getSize() {
		return size;
	}

	public void setDigest(String digest) {
		this.digest = digest;
	}

	public void setName(String name) {
		this.name = name;
	}

	public void setSize(int size) {
		this.size = size;
	}

	public String getSaveUrl() {
		return saveUrl;
	}

	public void setSaveUrl(String saveUrl) {
		this.saveUrl = saveUrl;
	}


}
