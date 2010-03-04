package com.incesoft.botplatform.sdk;

/**
 * A RobotFileDescriptor object contains attributes of a file when a file
 * transfer event received.
 * 
 * @author LiBo
 */
public interface RobotFileDescriptor {

	/**
	 * Get id of the current transfer. 
	 */
	public String getTransferId();

	/**
	 * Get name of the current transfer.
	 */
	public String getName();

	/**
	 * Get size of the current transfer.
	 */
	public long getSize();

	/**
	 * Get thumbnail of the current transfer (if available).
	 */
	public String getThumbnail();

}
