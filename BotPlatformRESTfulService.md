# Introduction #

Hi there,we suppose you are one of the service providers signed up in our website(http://botplatform.com) .

In this page, we will show you how to add, retrieve,modify and delete your resources in your own code.These resources you may have previously uploaded in our website.


# Details #

The RESTful Service we provided is simply a URL which can be used in your code through the HTTP methods(GET,POST,PUT and DELETE).

**The URL pattern is : http://botplatform.com/rest/{spid}/{type}/{name}**

**spid** : Your registered account in our website.

**type** : Resources' type for short, which should be one of the below.
  * dp -- display picture
  * ddp -- deluxe display picture
  * emo -- emoticons
  * voice -- voiceclip
  * wink
  * scene
  * file

**name** : Name of your resource.

**Here is an example URL : `http://botplatform.com/rest/SP106827/dp/dota.jpg`**

**Notice : When you make a call, you will encounter with a HTTP DIGEST authentication.**

# Example #

Here are some java examples implemented by `Apache HttpClient`.

**GET example**

```
HttpClient client = getHttpClient("UTF-8");
String getUrl = "http://botplatform.com/rest/SP106827/dp/"+ URLEncoder.encode("dota.jpg", "UTF-8");
try {
        GetMethod gm = new GetMethod(getUrl);
	Credentials ct = new UsernamePasswordCredentials("your spid","your passord");
	client.getState().setCredentials(AuthScope.ANY, ct);
	int status = client.executeMethod(gm);
	if (status == 200) {
		byte[] result = gm.getResponseBody();
                //process the result
	} else {
		System.out.println("error : " + status);
	}
} catch (HttpException ignored) {
} finally {
	gm.releaseConnection();
}
```

**POST example**
```
HttpClient client = getHttpClient("UTF-8");
String postUrl = "http://botplatform.com/rest/SP106827/dp/" + URLEncoder.encode("dota.jpg", "UTF-8");
try {
        PostMethod pm = new PostMethod(postUrl);
	FileInputStream is = new FileInputStream("your file path"); //may be a file you want to substitute for 'dota.jpg'
	ByteArrayOutputStream os = new ByteArrayOutputStream();
	byte[] buf = new byte[1024];
	int read = -1;
	while ((read = is.read(buf)) != -1) {
		os.write(buf, 0, read);
	}
	os.flush();
	os.close();
	pm.setRequestEntity(new ByteArrayRequestEntity(os.toByteArray()));
	Credentials ct = new UsernamePasswordCredentials("your spid", "your password");
	client.getState().setCredentials(AuthScope.ANY, ct);
	int status = client.executeMethod(pm);
	if (status == 200) {
		System.out.println("Update Success!");
	} else {
		System.out.println("error : " + status);
	}
} catch (HttpException ignored) {
} catch (IOException ignored) {
} finally {
	pm.releaseConnection();
}
```

PUT and DELETE examples are not listed here, but they are something similar, you may have it a go by yourself. :)