package com.incesoft.botplatform.sdk.protocol.msg;

import java.io.Serializable;
import java.util.HashMap;

public class TextMessage implements Serializable {

	private static final long serialVersionUID = -7399224891491114452L;

	private String signature;
	private int fontStyle;
	private String fontName;
	private int fontColor;
	private String text;
	private HashMap<String, String> emoticons;

	public String getSignature() {
		return signature;
	}

	public void setSignature(String signature) {
		this.signature = signature;
	}

	public int getFontStyle() {
		return fontStyle;
	}

	public void setFontStyle(int fontStyle) {
		this.fontStyle = fontStyle;
	}

	public String getFontName() {
		return fontName;
	}

	public void setFontName(String fontName) {
		this.fontName = fontName;
	}

	public int getFontColor() {
		return fontColor;
	}

	public void setFontColor(int fontColor) {
		this.fontColor = fontColor;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public HashMap<String, String> getEmoticons() {
		return emoticons;
	}

	public void setEmoticons(HashMap<String, String> emoticons) {
		this.emoticons = emoticons;
	}
}
