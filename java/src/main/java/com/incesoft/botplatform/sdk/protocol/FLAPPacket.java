package com.incesoft.botplatform.sdk.protocol;

public class FLAPPacket {
	
	public static final byte SIGNON = 1;
	public static final byte DATA = 2;
	public static final byte ERROR = 3;
	public static final byte SIGNOFF = 4;
	public static final byte KEEP_ALIVE = 5;

	private byte startMarker = 0x69;
	private byte frameType;
	private short sequenceNumber;
	private short payloadLength;
	private byte[] payload;
	
	public byte getStartMarker() {
		return startMarker;
	}
	public void setStartMarker(byte startMarker) {
		this.startMarker = startMarker;
	}
	public byte getFrameType() {
		return frameType;
	}
	public void setFrameType(byte frameType) {
		this.frameType = frameType;
	}
	public short getSequenceNumber() {
		return sequenceNumber;
	}
	public void setSequenceNumber(short sequenceNumber) {
		this.sequenceNumber = sequenceNumber;
	}
	public short getPayloadLength() {
		return payloadLength;
	}
	public void setPayloadLength(short payloadLength) {
		this.payloadLength = payloadLength;
	}
	public byte[] getPayload() {
		return payload;
	}
	public void setPayload(byte[] payload) {
		this.payload = payload;
	}
}
