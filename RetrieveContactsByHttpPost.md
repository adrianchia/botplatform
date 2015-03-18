# Introduction #

You may have noticed that in the SDK there's a method called 'requestContactList' in 'RobotServer' class, now we provide another way to do this -- through HTTP Post.
We provided this as an enhanced function to retrieve your accounts' contacts. 'Enhanced' means it can retrieve contacts of any account as long as you provide the correct corresponding password, not just the robot accounts which you previously bound with our platform.


# Details #

Let's take a look at the URl and the Name-Value params for making the HTTP Post request.

**URL : http://botplatform.com/rest/contacts**

**Params :**
  * account - the account to be retrieved
  * pwd - the account's password, but should be Triple DES encrypted by the key generated from your SP's password.(Discuss later)
  * spid - your SPID
  * type - type of the account. (Now only msn is supported, so this param could be omitted)

The 'pwd' param is something complicated. As the key for Triple DES encription should be 24 bytes, please transfer your sp's password into byte array first, then if it's length is greater than 24, truncate the byte array from the beginning, if the length is less than 24, please pad the rest with '0x00'. When the key is ready, then do the Triple DES encription with it. **Please add 8 bytes IV parameter before the cipher bytes, which makes the ciphertext is not the same each time.**

**Note : HTTP digest authentication is needed when you make a POST request.(use your spid and password)**

Finally, let's see the HTTP response's content, format in JSON.

_Example :_`[{"account":"abc@msn.com","displayName":"name ABC"},{"account":"def@hotmail.com","displayName":"name DEF"},{......}]`

**Java Example Code:**
```
// Generate key by Sp's password
byte[] rawBytes = sppwd.getBytes();
byte[] keybytes = new byte[24];
if (rawBytes.length == 24)
	keybytes = rawBytes;
else {
	int copyLen = 0;
	if (rawBytes.length > 24)
		copyLen = 24;
	else
		copyLen = rawBytes.length;
	System.arraycopy(rawBytes, 0, keybytes, 0, copyLen);
}
try {
	KeySpec spec = new DESedeKeySpec(keybytes);
	SecretKeyFactory fac = SecretKeyFactory.getInstance("DESEde");
	java.security.Key key = fac.generateSecret(spec);
} catch (Exception ignored) {
}


// Encrypt with the key
byte[] bytesFinal = null;
Cipher cipher;
byte[] result = null;
try {
	cipher = Cipher.getInstance("DESede/CBC/PKCS5Padding");
	byte[] ivbytes = new byte[8];
	SecureRandom sr = new SecureRandom();
	sr.nextBytes(ivbytes);
	IvParameterSpec iv = new IvParameterSpec(ivbytes);
	cipher.init(Cipher.ENCRYPT_MODE, key, iv);
	bytesFinal = cipher.doFinal(raw);
	result = new byte[bytesFinal.length + 8];
	System.arraycopy(ivbytes, 0, result, 0, 8);
	System.arraycopy(bytesFinal, 0, result, 8, bytesFinal.length);
} catch (Exception e) {
	e.printStackTrace();
} finally {
	cipher = null;
}


// Post Request By Apache HttpClient
HttpClient client = getHttpClient("UTF-8");
PostMethod pm = new PostMethod("http://botplatform.com/rest/contacts");
try {
	pm.setParameter("account", "your im account");
	pm.setParameter("spid", "your spid");
	DesEncrypt de = new DesEncrypt(); // triple des encryption utility class
	byte[] pwdBytes = "your sp's password".getBytes();
	byte[] key = new byte[24]; // 24 bytes key
	System.arraycopy(pwdBytes, 0, key, 0, pwdBytes.length);
	de.setKey(key);
	pm.setParameter("pwd", de.encrypt("your account's password"));
	Credentials ct = new UsernamePasswordCredentials("your spid",
			"your sp's password");
	client.getState().setCredentials(AuthScope.ANY, ct); // http digest authentication
	int status = client.executeMethod(pm);
	if (status == 200) {
		byte[] result = pm.getResponseBody();
		System.out.println(new String(result, "UTF-8"));
	} else {
		System.out.println(pm.getStatusText());
	}
} catch (Exception ignored) {
} finally {
	pm.releaseConnection();
}
```