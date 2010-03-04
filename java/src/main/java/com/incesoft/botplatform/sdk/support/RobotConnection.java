package com.incesoft.botplatform.sdk.support;

import java.io.UnsupportedEncodingException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.util.Random;
import java.util.concurrent.ExecutorService;

import org.apache.mina.common.IdleStatus;
import org.apache.mina.common.IoConnector;
import org.apache.mina.common.IoHandler;
import org.apache.mina.common.IoSession;

import com.incesoft.botplatform.sdk.RobotException;
import com.incesoft.botplatform.sdk.protocol.FLAPPacket;
import com.incesoft.botplatform.sdk.protocol.json.JsonUtil;
import com.incesoft.botplatform.sdk.protocol.msg.Message;


public abstract class RobotConnection implements IoHandler {
	
	private static class Seq {
		private short seq;
		public Seq() {
			seq = (short)new Random().nextInt(Short.MAX_VALUE);
		}
		public synchronized short next() {
			return seq++;
		}
	}
	
	protected ExecutorService executor = null;
    protected IoConnector connector = null;
    protected IoSession session = null;
    protected SocketAddress socketAddress;
    protected Seq seq = new Seq();
    
    public void open(String host,int port) {
    	SocketAddress socketAddress = new InetSocketAddress(host, port);
        connector.connect(socketAddress, this);
    }
    public void close() {
		if(session != null)session.close();
	}
    public void keepAlive() {
		if(session != null) {
			FLAPPacket packet = new FLAPPacket();
			packet.setFrameType(FLAPPacket.KEEP_ALIVE);
			packet.setSequenceNumber(seq.next());
			session.write(packet);
		}
	}
    public void sendMessage(Message msg) {
		if(session != null) {
			try {
				FLAPPacket packet = new FLAPPacket();
				packet.setFrameType(FLAPPacket.DATA);
				packet.setSequenceNumber(seq.next());
				String json = JsonUtil.toJson(msg);
				byte[] jsonBytes = json.getBytes("UTF-8");
				packet.setPayloadLength((short)jsonBytes.length);
				packet.setPayload(jsonBytes);
				session.write(packet);
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
		}
	}
    
    protected abstract void processOpened() throws Exception ;
    protected abstract void processClosed() throws Exception ;
    protected abstract void processError(Throwable e) throws RobotException;
    protected abstract void processMessage(Message msg) throws Exception ;
    protected abstract void processSent() throws Exception;
    
    public void sessionOpened(IoSession session) throws Exception {
		this.session = session;
		processOpened();
	}
	public void sessionClosed(IoSession arg0) throws Exception {
		processClosed();
		this.session = null;
	}

	public void exceptionCaught(IoSession session, Throwable e)
			throws Exception {
		processError(e);
	}

	public void messageReceived(IoSession session, final Object obj) throws Exception {
		executor.execute(new Runnable() {
			public void run() {
				try {
					FLAPPacket packet = (FLAPPacket)obj;
					if(packet.getFrameType() == FLAPPacket.DATA) {
						byte[] payload = packet.getPayload();
						if(payload != null && payload.length>0) {
							Message msg = JsonUtil.fromJson(new String(payload,"UTF-8"));
							processMessage(msg);
						}
					}
				} catch (Throwable t) {
					try {
						processError(t);
					} catch (RobotException e) {
						e.printStackTrace();
					}
				}
			}
		});
	}

	public void messageSent(IoSession arg0, Object arg1) throws Exception {
		processSent();
	}

	

	public void sessionCreated(IoSession arg0) throws Exception {
		
	}

	public void sessionIdle(IoSession arg0, IdleStatus arg1) throws Exception {
		
	}

	

	public ExecutorService getExecutor() {
		return executor;
	}

	public void setExecutor(ExecutorService executor) {
		this.executor = executor;
	}

	public IoConnector getConnector() {
		return connector;
	}

	public void setConnector(IoConnector connector) {
		this.connector = connector;
	}

}
