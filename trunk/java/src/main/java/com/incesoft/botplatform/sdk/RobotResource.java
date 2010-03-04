package com.incesoft.botplatform.sdk;

/**
 * Describes a resource when robot receives a corresponding event.
 * (eg. when a wink received)
 * @author LiBo
 */
public interface RobotResource {

	/**
	 * Get the name of the resource
	 */
	public String getName();
	
	/**
	 * Get the digest of the resource
	 */
	public String getDigest();
	
	/**
	 * Get the size of the resource
	 */
	public int getSize();
	
}
