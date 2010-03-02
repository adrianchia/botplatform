package com.incesoft.botplatform.sdk.protocol.json;

import java.lang.reflect.Type;

import com.google.gson.JsonDeserializationContext;
import com.google.gson.JsonDeserializer;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParseException;
import com.incesoft.botplatform.sdk.protocol.msg.Message;

@SuppressWarnings("unchecked")
public class MessageDeserializer implements JsonDeserializer {

	public Object deserialize(JsonElement el, Type type,
			JsonDeserializationContext ctx) throws JsonParseException {
		JsonObject obj = el.getAsJsonObject();
		Message msg = new Message();
		if (null != obj.get("robotId"))
			msg.setRobotId(obj.get("robotId").getAsString());
		if (null != obj.get("userId"))
			msg.setUserId(obj.get("userId").getAsString());
		if (null != obj.get("sessionId"))
			msg.setSessionId(obj.get("sessionId").getAsString());
		msg.setType(obj.get("type").getAsString());

		JsonElement bodyElement = obj.get("body");
		if (bodyElement != null) {
			Class clazz = Message.mapBody(msg.getType());
			if (clazz != null)
				msg.setBody(ctx.deserialize(bodyElement, clazz));
		}
		return msg;
	}

}
