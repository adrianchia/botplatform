package com.incesoft.botplatform.sdk.protocol;

import org.apache.mina.common.ByteBuffer;
import org.apache.mina.common.IoSession;
import org.apache.mina.filter.codec.ProtocolEncoder;
import org.apache.mina.filter.codec.ProtocolEncoderOutput;


public class FLAPEncoder implements ProtocolEncoder {

	
	public void encode(IoSession session, Object obj, ProtocolEncoderOutput out) throws Exception {
		
		FLAPPacket packet = (FLAPPacket)obj;
		
		ByteBuffer buf = ByteBuffer.allocate(6+packet.getPayloadLength());
		buf.put(packet.getStartMarker());
		buf.put(packet.getFrameType());
		buf.putShort(packet.getSequenceNumber());
		buf.putShort(packet.getPayloadLength());
		if(packet.getPayloadLength()>0)
			buf.put(packet.getPayload());
		buf.flip();
		out.write(buf);

	}
	
	public void dispose(IoSession arg0) throws Exception {
		// TODO Auto-generated method stub
		
	}
}
