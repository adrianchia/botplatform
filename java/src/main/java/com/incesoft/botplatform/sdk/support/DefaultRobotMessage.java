/*
 * Created on 2006-2-21
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk.support;


import java.util.HashMap;

import com.incesoft.botplatform.sdk.RobotMessage;
import com.incesoft.botplatform.sdk.protocol.msg.TextMessage;

/**
 * @author LiBo
 */
public class DefaultRobotMessage implements RobotMessage {

    private TextMessage message;
    
    public DefaultRobotMessage() {
        message = new TextMessage();
    }
    
    public DefaultRobotMessage(TextMessage message) {
        this.message = message;
    }

    public String getString() {
        return message.getText();
    }

    public RobotMessage setString(String content) {
        message.setText(content);
        return this;
    }

    public int getInt() throws NumberFormatException {
        return Integer.parseInt(getString());
    }

    public RobotMessage setInt(int content) {
        setString("" + content);
        return this;
    }

    
    public String getSignature() {
        return (String)message.getSignature();
    }
    public RobotMessage setSignature(String sig) {
        message.setSignature(sig);
        return this;
    }
    
    public String getFontName() {
        return message.getFontName();
    }

    public RobotMessage setFontName(String name) {
        message.setFontName(name);
        return this;
    }

    public int getFontStyle() {
        return message.getFontStyle();
    }

    public RobotMessage setFontStyle(int style) {
        message.setFontStyle(style);
        return this;
    }

    public int getFontColor() {
        return message.getFontColor();
    }

    public RobotMessage setFontColor(int color) {
        message.setFontColor(color);
        return this;
    }

    public void registerEmoticon(String shortcut, String filename) {
    	HashMap<String,String> emos = message.getEmoticons();
    	if(emos == null) {
    		emos = new HashMap<String,String>();
    		message.setEmoticons(emos);
    	}
    	emos.put(shortcut,filename);
    }

    public void deregisterEmoticon(String shortcut) {
    	HashMap<String,String> emos = message.getEmoticons();
    	if(emos != null)
    		emos.remove(shortcut);
    }
    
    public TextMessage getProtocolMessage() {
        return message;
    }


}
