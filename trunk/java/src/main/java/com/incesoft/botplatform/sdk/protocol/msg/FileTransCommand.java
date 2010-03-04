package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

public class FileTransCommand implements Serializable{
	private static final long serialVersionUID = 6110926541489833753L;
	private String cmd;
	private String transferId;
	private String saveUrl;
	public String getCmd() {
		return cmd;
	}
	public void setCmd(String cmd) {
		this.cmd = cmd;
	}
	public String getTransferId() {
		return transferId;
	}
	public void setTransferId(String transferId) {
		this.transferId = transferId;
	}
	public String getSaveUrl() {
		return saveUrl;
	}
	public void setSaveUrl(String saveUrl) {
		this.saveUrl = saveUrl;
	}
}
