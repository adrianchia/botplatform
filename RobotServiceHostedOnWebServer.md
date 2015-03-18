In this page, we will introduce a new feature to you which named 'AppEngine'.

As you may not have your own server which allows you to connect to our platform directly by using our SDK, now there is another way to go, what you need is just a host on which your programs are deployed, and specify the URL in the admin page.(Use your SPID and password and sign in on http://botplatform.com, you will see the 'Web Host Service' option in 'Basic Information ').

**Note: If your SDK program already connects while you also provide the service for AppEngine, SDK is preferred.**

# Details #

When you get ready for this, we will transer XML data through HTTP POST to the specified URL and get the HTTP response (also in XML format) to perform actions.

**We will post almost all of the 'Session Level' messages between your bot and your users to the specified URL. The XML's structure is clear and easy to understand (You may compare it with its corresponding Class file we difined in SDK), the 'type' attribute indicates the kind of the current message.**

_Examples of our XML data for POST:_

  * TextMessage :
```
<request>
  <robotId>bot@hotmail.com</robotId>
  <userId>abc@hotmail.com</userId>
  <sessionId>5257573032606033981</sessionId>
  <action type="msg">
    <fontStyle>0</fontStyle>
    <fontName>Tohoma</fontName>
    <fontColor>0</fontColor>
    <text>some plain text</text>
  </action>
</request>
```

  * File Message :
```
<request>
  <robotId>bot@hotmail.com</robotId>
  <userId>abc@hotmail.com</userId>
  <sessionId>5257573032606033981</sessionId>
  <action type="fileevent">
    <event>invite</event>
    <transferId>{B1D11ABA-8608-45A5-BD91-1F1ABC685639}</transferId>
    <size>4998</size>
    <name>wiki.txt</name>
  </action>
</request>
```

  * Wink Message :
```
<request>
  <robotId>bot@hotmail.com</robotId>
  <userId>abc@hotmail.com</userId>
  <sessionId>5257573032606033981</sessionId>
  <action type="winkevent">
    <digest>Wink digest</digest>
    <name>Wink Name</name>
    <size>30900</size>
  </action >
</request>
```

  * Ink Message :
```
<request>
  <robotId>bot@hotmail.com</robotId>
  <userId>abc@hotmail.com</userId>
  <sessionId>5257573032606033981</sessionId>
  <action type="inkmsg">ink data here</action>
</request>
```

  * Nudge Message :
```
<request>
  <robotId>bot@hotmail.com</robotId>
  <userId>abc@hotmail.com</userId>
  <sessionId>5257573032606033981</sessionId>
  <action type="nudge" />
</request>
```

  * Application Window Message :
```
<request>
  <robotId>bot@hotmail.com</robotId>
  <userId>abc@hotmail.com</userId>
  <sessionId>5257573032606033981</sessionId>
  <action type="appevent">accept</action>
</request>
```


**When posted, we expect the HTTP response. You may perform several actions per message(a HTTP request) if you obey the XML definition shown blow.**


_Definition of the XML structure in HTTP response:_
```
<response>
    <actions>
	<action type="nudge" /><!-- send nudge -->
	<action type="typing" /><!-- send typing -->
	<action type="msg"><!-- send text message -->
		<fontColor>CCCCCC</fontColor>
		<fontStyle>10</fontStyle>
		<fontName>Tahoma</fontName>
		<text>hello(1) world(2)</text>
		<emoticons>
			<entry key="(1)" value="emo1.png" />
			<entry key="(2)" value="emo2.png" />
		</emoticons>
	</action>
	<action type="wink"><!-- send specified wink -->
		<name>wink.cab</name>
		<stamp>stamp of the wink, can be empty</stamp>
	</action>
	<action type="ink"><!-- send ink message -->
		<data>base64 encoded ink data</data>
	</action>
	<action type="voice"><!-- send specified voiceclip -->
		<name>voiceclip.wav</name>
	</action>
	<action type="filecmd"><!-- when a user sends a file to your bot -->
		<cmd>accept</cmd><!-- accept or reject -->
		<transferId>tid</transferId>
		<saveUrl>http://example.saveurl.com</saveUrl><!-- for accept command only -->
	</action>
	<action type="file"><!-- send a specified file to your users -->
		<name>file.pdf</name>
	</action>
	<action type="app"><!-- open activity window -->
		<name>BotPlatform</name>
		<data>http://botplatform.com</data>
	</action>
    </actions>
</response>
```

