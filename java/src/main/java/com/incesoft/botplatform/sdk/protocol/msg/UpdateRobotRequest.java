package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;

public class UpdateRobotRequest implements Serializable{
	private static final long serialVersionUID = -4694646733136953245L;
	private int status = -1;
	private String displayName;
	private String personalMessage;
	private String displayPicture;
	private String largePicture;
	private String scene;
	private int colorScheme = -1;

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

	public String getDisplayName() {
		return displayName;
	}

	public void setDisplayName(String displayName) {
		this.displayName = displayName;
	}

	public String getPersonalMessage() {
		return personalMessage;
	}

	public void setPersonalMessage(String personalMessage) {
		this.personalMessage = personalMessage;
	}

	public String getDisplayPicture() {
		return displayPicture;
	}

	public void setDisplayPicture(String displayPicture) {
		this.displayPicture = displayPicture;
	}

	public String getLargePicture() {
		return largePicture;
	}

	public void setLargePicture(String largePicture) {
		this.largePicture = largePicture;
	}

	public String getScene() {
		return scene;
	}

	public void setScene(String scene) {
		this.scene = scene;
	}

	public int getColorScheme() {
		return colorScheme;
	}

	public void setColorScheme(int colorScheme) {
		this.colorScheme = colorScheme;
	}

}
