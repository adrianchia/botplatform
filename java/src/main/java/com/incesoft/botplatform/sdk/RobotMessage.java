/*
 * Created on 2006-2-20
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk;


/**
 * This class represents a msn chat message.
 * @author LiBo
 */
public interface RobotMessage {
    
    /**
     * Font style - bold
     */
    public static final int STYLE_BOLD = 1;
    /**
     * Font style - italic
     */
    public static final int STYLE_ITALIC = 2;
    /**
     * Font style - underline
     */
    public static final int STYLE_UNDERLINE = 4;
    /**
     * Font style - strikeout
     */
    public static final int STYLE_STRIKE = 8;
    /**
     * Font style - bold + underline 
     */
    public static final int STYLE_BOLD_ITALIC = 3;
    /**
     * Font style - bold + italic + strikeout + underline
     */
    public static final int STYLE_ALL = 15;
    
    /**
     * Get the plain text of the message
     */
    public String getString();
    /**
     * Set the plain text of the message
     * @return RobotMessage 
     */
    public RobotMessage setString(String content);
    
    /**
     * A util function to get integer content. 
     * When the content is an integer, 
     * this function will return the parsed integer 
     *  using <code>Integer.parseInt(getString())</code>
     */
    public int getInt() throws NumberFormatException;
    /**
     * A util function to set the message text with integer content
     *  using <code>setString(String.valueOf(content))</code>
     * @return RobotMessage back to the set message 
     */
    public RobotMessage setInt(int content);
    
    /**
     * Get robot message preface. eg: xxx says.
     */
    public String getSignature();
    /**
     * Set robot message preface
     * @return RobotMessage
     */
    public RobotMessage setSignature(String sig);
    
    /**
     * Get font name of the message
     */
    public String getFontName();
    /**
     * Set font name of the message
     * @return RobotMessage 
     */
    public RobotMessage setFontName(String name);
    
    /**
     * Get font style of the message, as bold, italic, strikeout, underline
     */
    public int getFontStyle();
    /**
     * Set font style of the message, as bold, italic,strikeout, underline
     * @return RobotMessage 
     */
    public RobotMessage setFontStyle(int style);
    
    /**
     * Get font color of the message
     */
    public int getFontColor();
    /**
     * Set font color of the message
     * @return RobotMessage 
     */
    public RobotMessage setFontColor(int color);
    
    /**
     * Register custom emoticon
     * @param shortcut The shortcut of the emoticon. If shortcut appears in message, it'll show as an icon corresponding to your 'filename'. 
     * @param filename The filename of emotion. Submitted to INCE by SP, and a certified file of INCE 
     */
    public void registerEmoticon(String shortcut, String filename);
    
    /**
     * Deregister the emoticon 
     * After the invocation of this function, the shortcut appearing in the message will show as plain text.   
     */
    public void deregisterEmoticon(String shortcut);
    
}
