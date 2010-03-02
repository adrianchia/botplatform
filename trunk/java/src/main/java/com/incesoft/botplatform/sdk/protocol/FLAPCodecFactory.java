package com.incesoft.botplatform.sdk.protocol;

import org.apache.mina.filter.codec.ProtocolCodecFactory;
import org.apache.mina.filter.codec.ProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolEncoder;

public class FLAPCodecFactory implements ProtocolCodecFactory {

	public ProtocolDecoder getDecoder() throws Exception {
		return new FLAPDecoder();
	}

	public ProtocolEncoder getEncoder() throws Exception {
		return new FLAPEncoder();
	}

}
