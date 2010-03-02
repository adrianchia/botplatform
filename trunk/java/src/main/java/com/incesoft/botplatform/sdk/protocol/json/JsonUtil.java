package com.incesoft.botplatform.sdk.protocol.json;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.incesoft.botplatform.sdk.protocol.msg.Message;

public class JsonUtil {
	
	private static MessageDeserializer deserializer = new MessageDeserializer();
	
	public static Message fromJson(String json) {
		Gson gson = new GsonBuilder().registerTypeAdapter(Message.class, deserializer).create();
		return gson.fromJson(json, Message.class);
	}
	
	public static String toJson(Message msg){
		return new Gson().toJson(msg);
	}
}
