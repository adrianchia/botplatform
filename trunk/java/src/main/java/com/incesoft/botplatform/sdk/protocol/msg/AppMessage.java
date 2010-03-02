package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

public class AppMessage implements Serializable {

	private static final long serialVersionUID = 8677546141320265709L;

	private String id;
	private String name;
	private String data;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getData() {
		return data;
	}

	public void setData(String data) {
		this.data = data;
	}

}
