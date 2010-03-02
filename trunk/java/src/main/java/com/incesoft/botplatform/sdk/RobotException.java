/*
 * Created on 2006-2-20
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.incesoft.botplatform.sdk;

/**
 * @author LiBo
 */
public class RobotException extends Exception {

	private static final long serialVersionUID = 1L;
	
	public static final int ERROR_LOCAL = 0 ;
    public static final int ERROR_EXCEED_CONNECTION_LIMIT = 101 ;
    public static final int ERROR_INVALID_SPID = 102 ;
    public static final int ERROR_SERVER_ERROR = 103 ;
    public static final int ERROR_ACCOUNT_NOT_BOUND = 104 ;
    public static final int ERROR_REQUEST_TOO_FREQUENTLY = 105 ;
    
    public static final int ERROR_NO_IDLE_SERVER = 201 ;
    public static final int ERROR_RESOURCE_NOT_FOUND = 301 ;
    public static final int ERROR_PRIVILEGE_LIMITED = 401 ;
    public static final int ERROR_INVITE_USER_FAILED = 501 ;
    public static final int ERROR_CREATE_SESSION_FAILED = 502 ;
    public static final int ERROR_CALL_TOO_FREQUENTLY = 503 ;
    
    public static final int ERROR_PROVISION_ACCOUNT_DOESNOT_SUPPORT = 601 ;

	public static final int ERROR_DATA_LENGTH_LIMITED = 504;
    
    private int errorCode = ERROR_LOCAL;
    
    /**
     *
     */

    public RobotException() {
        super();
        // TODO Auto-generated constructor stub
    }
    /**
     *
     */

    public RobotException(String message) {
        super(message);
        // TODO Auto-generated constructor stub
    }
    
    public RobotException(int code, String message) {
        super(message);
        this.errorCode = code;
        // TODO Auto-generated constructor stub
    }
    /**
     *
     */

    public RobotException(String message, Throwable cause) {
        super(message, cause);
        // TODO Auto-generated constructor stub
    }
    /**
     *
     */

    public RobotException(Throwable cause) {
        super(cause);
        // TODO Auto-generated constructor stub
    }
	public int getErrorCode() {
		return errorCode;
	}
	public void setErrorCode(int errorCode) {
		this.errorCode = errorCode;
	}

    
}
