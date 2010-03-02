package com.incesoft.botplatform.sdk.protocol;


import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.mina.common.ByteBuffer;
import org.apache.mina.common.IoSession;
import org.apache.mina.filter.codec.ProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolDecoderOutput;

public class FLAPDecoder implements ProtocolDecoder {
	
	private static Log log = LogFactory.getLog(FLAPDecoder.class);
	
	private static final String REMAINING_BUFFER_KEY = "flap.remainingbuf";
	private static final String INCOMING_PACKET_KEY = "flap.incomingpacket";
	

	public void decode(IoSession session, ByteBuffer currentBuf, ProtocolDecoderOutput out) throws Exception {
		
    	ByteBuffer receiveBuf = null;
    	try {
    		
	    	byte[] remainingBuf = (byte[])session.removeAttribute(REMAINING_BUFFER_KEY);
	    	FLAPPacket packet = (FLAPPacket)session.removeAttribute(INCOMING_PACKET_KEY);
	    	
	    	if (remainingBuf != null) {
	    		receiveBuf = ByteBuffer.allocate(remainingBuf.length + currentBuf.remaining());
	    		receiveBuf.put(remainingBuf);
	    		receiveBuf.put(currentBuf.buf());
	    		receiveBuf.flip();
	    	}
	    	else {
	    		receiveBuf = currentBuf;
	    	}
        	
            while (receiveBuf.hasRemaining()) {
            
            	if (packet == null) {
            		
            		if (receiveBuf.remaining()<8) break;
            		
                	packet = new FLAPPacket();
                	packet.setStartMarker(receiveBuf.get());
                	packet.setFrameType(receiveBuf.get());
                	packet.setSequenceNumber(receiveBuf.getShort());
                	packet.setPayloadLength(receiveBuf.getShort());
            	}
                
            	if(receiveBuf.remaining()<packet.getPayloadLength())break;
            	
            	if(packet.getPayloadLength()>0) {
	            	byte[] payload = new byte[packet.getPayloadLength()];
	            	receiveBuf.get(payload);
	            	packet.setPayload(payload);
            	}
            	
            	out.write(packet);

            	packet = null; //must be set to null
            }
            
            if(receiveBuf.remaining()>1048576) throw new IllegalStateException("remainingBuf > 1M");
            
            if (receiveBuf.hasRemaining()) {
            	remainingBuf = new byte[receiveBuf.remaining()];
            	receiveBuf.get(remainingBuf);
            	session.setAttribute(REMAINING_BUFFER_KEY,remainingBuf);
            }
            if (packet != null) {
            	session.setAttribute(INCOMING_PACKET_KEY,packet);
            }
        } catch (Throwable e) {
        	log.error(e.getMessage(),e);
            session.close();
        } finally {
        	if (receiveBuf != currentBuf) {
        		receiveBuf.release();
        	}
        }
		
	}
	
	public void dispose(IoSession arg0) throws Exception {
		// TODO Auto-generated method stub
		
	}

	public void finishDecode(IoSession arg0, ProtocolDecoderOutput arg1)
			throws Exception {
		// TODO Auto-generated method stub
		
	}

	
}
