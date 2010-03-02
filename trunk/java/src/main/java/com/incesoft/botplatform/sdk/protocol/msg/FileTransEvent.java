package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

import com.incesoft.botplatform.sdk.RobotFileDescriptor;

public class FileTransEvent implements RobotFileDescriptor, Serializable {

	private static final long serialVersionUID = -4532454284595089533L;

	private String event;
	private String transferId;
	private long size;
	private String name;
	private String thumbnail;

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public long getSize() {
		return size;
	}

	public void setSize(long size) {
		this.size = size;
	}

	public String getThumbnail() {
		return thumbnail;
	}

	public void setThumbnail(String thumbnail) {
		this.thumbnail = thumbnail;
	}

	public String getTransferId() {
		return transferId;
	}

	public void setTransferId(String transferId) {
		this.transferId = transferId;
	}

	public String getEvent() {
		return event;
	}

	public void setEvent(String event) {
		this.event = event;
	}

}
