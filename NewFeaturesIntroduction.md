# Introduction #

In this page, we're going to introduce the new features of our latest SDK(Java version).You may also check out these difference by yourself through our documentations.


# Details #

### 1.RobotServer ###

First let's take a quick look over the three methods listed below:
```
public void setDisplayPictureEx(String displayPicture, String deluxePicture) throws RobotException;
public void pushMessage(String robot, String user, RobotMessage message) throws RobotException;
public void requestResource(String robot, String user, RobotResource resource, String saveUrl) throws RobotException ;
```

  * The **setDisplayPictureEx** method's second argument represents a larger display picture to show, so we call it a deluxe picture.
  * The signature of **pushMessage** method is changed, now it accepts a RobotMessage object, so you can specify your favourite font's style and color.
  * **requestResource** is the most interesting method to be introduced.What does a resource represent for? When you receive a wink or a voiceclip, they are the resources which you can request to get them.How? Just specify the **saveUrl** argument, then we will post the data to this saveUrl.You may also specify it as **null**, we'll provide saveUrl for you(To know more about this, please visit http://code.google.com/p/botplatform/wiki/BotPlatformRESTfulService).

### 2.RobotSession ###

```
public void sendFileAcceptance(String transferId, String saveUrl) throws RobotException;
public void sendFileRejection(String transferId)  throws RobotException;
public void sendFileCancellation(String transferId)  throws RobotException;

public void sendInk(byte[] inkData) throws RobotException;
```

  * Notice the three methods ralated to file transfer.When a user of your robot sends a file, you can make some actions to meet your demands.
  * **sendInk** method now makes you simply and directly specify binary data to be sent.

### 3.RobotHandler ###

Now this class is more functional than previous one. We don't explain these events here as you will get well informed when you take a look at them.

```
public void resourceReceived(String robot, String user,RobotResource resource, String saveUrl);
public void colorSchemeUpdated(String robot, String user, int colorScheme) throws RobotException;
public void fileTransferCancelled(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;
public void fileTransferError(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;
public void fileReceived(String robot, String user, RobotFileDescriptor fileDescriptor, String saveUrl);
public void fileInvited(RobotSession session, RobotFileDescriptor fileDescriptor) throws RobotException;
public void displayPictureUpdated(String robot, String user, RobotResource resource) throws RobotException;
public void sceneUpdated(String robot, String user, RobotResource resource) throws RobotException;
public void winkReceived(RobotSession session, RobotResource resource) throws RobotException;
public void inkReceived(RobotSession session,byte[] ink) throws RobotException;
public void voiceclipReceived(RobotSession session,RobotResource resource) throws RobotException;
public void typingReceived(RobotSession session) throws RobotException;
```

If you have any problem in working with these events, please let us know ASAP.