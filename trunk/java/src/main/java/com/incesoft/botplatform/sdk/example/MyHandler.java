package com.incesoft.botplatform.sdk.example;

import java.util.List;
import java.util.Random;

import com.incesoft.botplatform.sdk.RobotException;
import com.incesoft.botplatform.sdk.RobotHandler;
import com.incesoft.botplatform.sdk.RobotMessage;
import com.incesoft.botplatform.sdk.RobotResource;
import com.incesoft.botplatform.sdk.RobotServer;
import com.incesoft.botplatform.sdk.RobotSession;
import com.incesoft.botplatform.sdk.RobotFileDescriptor;
import com.incesoft.botplatform.sdk.RobotUser;

public class MyHandler implements RobotHandler {

	public final String commandList = "*****************************************\r"
			+ "**  BOTPLATFORM SDK DEMO COMMAND LIST  **\r"
			+ "*****************************************\r"
			+ " preface --- test message preface. \r"
			+ " emo ------- test emoticon.\r"
			+ " typing ---- test typing info.\r"
			+ " nudge ----- test nudge.\r"
			+ " ink ------- test ink.\r"
			+ " wink ------ test wink.\r"
			+ " voice ----- test voiceclip.\r"
			+ " file ------ test file transfer.\r"
			+ " p4 -------- test msn activity.\r"
			+ " webcam ---- test webcam.\r"
			+ " name ------ set friendly name.\r"
			+ " psm ------- set personal message.\r"
			+ " dp -------- set display picture.\r"
			+ " ddp ------- set delux display picture.\r"
			+ " scene ----- set scene.\r"
			+ " color ----- set color scheme.\r"
			+ " invite ---- invite user.\r"
			+ " push ------ push message.\r"
			+ " create ---- create session.\r"
			+ " close ----- close session.\r"
			+ " help ------ print this command list.\r"
			+ " ? --------- print this command list.\r";

	private RobotServer server = null;

	public MyHandler(RobotServer server) {
		this.server = server;
	}

	public void sessionOpened(RobotSession session) {
		debugEvent("user=" + session.getUser().getID() + ",friendly="
				+ session.getUser().getFriendlyName() + ",clientId="
				+ session.getUser().getClientID() + ",status="
				+ session.getUser().getStatus());
		try {
			if (RobotSession.OPEN_MODE_CONV_OPEN == session.getOpenMode()) {
				session.send(commandList);
			} 
		} catch (RobotException e) {
			e.printStackTrace();
		}
	}

	public void sessionClosed(RobotSession session) {
		debugEvent();
	}

	public void messageReceived(RobotSession session, RobotMessage message) {

		debugEvent("message=" + message.getString());
		
		try {
			String command = message.getString();
			String param = null;
			int ii = command.indexOf(":");
			if(ii>0) {
				param = command.substring(ii+1);
				command = command.substring(0,ii);
			}

			if ("help".equals(command)
					|| "?".equals(command)) {
				session.send(commandList);
			} else if ("preface".equals(command)) {
				RobotMessage msg = session.createMessage();
				msg.setSignature("preface-" + new Random().nextInt(100));
				msg.setString("test change preface");
				session.send(msg);
			} else if ("emo".equals(command)) {
				RobotMessage msg = session.createMessage();
				msg.registerEmoticon("(1)", "bear.png");
				msg.registerEmoticon("(2)", "beaver.png");
				msg.registerEmoticon("(3)", "balloon.png");
				msg.setString("a(1)b(2)c(3)d");
				session.send(msg);
			} else if ("typing".equalsIgnoreCase(command)) {
				session.sendTyping();
			} else if ("nudge".equalsIgnoreCase(command)) {
				session.sendNudge();
			} else if ("wink".equals(command)) {
				String k = "MIIIngYJKoZIhvcNAQcCoIIIjzCCCIsCAQExCzAJBgUrDgMCGgUAMCwGCSqGSIb3DQEHAaAfBB1SZ016K2JpeU1RSkxEeGxIWFVoZ0FOdFhpZDg9YaCCBrUwggaxMIIFmaADAgECAgoJlhkGAAEAAADYMA0GCSqGSIb3DQEBBQUAMHwxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpXYXNoaW5ndG9uMRAwDgYDVQQHEwdSZWRtb25kMR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xJjAkBgNVBAMTHU1TTiBDb250ZW50IEF1dGhlbnRpY2F0aW9uIENBMB4XDTA2MDQwMTIwMDI0NVoXDTA2MDcwMTIwMTI0NVowUTESMBAGA1UEChMJTWljcm9zb2Z0MQwwCgYDVQQLEwNNU04xLTArBgNVBAMTJDM0ZmE4MmIyLWZkYTAtNDhkYS04Zjk1LWZjNjBkNWJhYjgyOTCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA45cPz9tVdVnx4ATC0sXxMKMfpzOXvs6qs1d/Z8Pcp3Wr2ovHTd/pRd6Vn8ss/MqTL3hDPxaV+4w4TJCpfoDiCH1H4lwoshw0dY2/eOiWJgd2ONyiJ7dEvStCqrs+QliZVEaGwDjlsh17pHOrBRAA6WBo7TIeiTANpjLn+HkJm80CAwEAAaOCA+IwggPeMB0GA1UdDgQWBBT7ea5Y7aSMXkVnAEDgvXadh5LVSzAfBgNVHSUEGDAWBggrBgEFBQcDCAYKKwYBBAGCNzMBAzCCAksGA1UdIASCAkIwggI+MIICOgYJKwYBBAGCNxUvMIICKzBJBggrBgEFBQcCARY9aHR0cHM6Ly93d3cubWljcm9zb2Z0LmNvbS9wa2kvc3NsL2Nwcy9NaWNyb3NvZnRNU05Db250ZW50Lmh0bTCCAdwGCCsGAQUFBwICMIIBzh6CAcoATQBpAGMAcgBvAHMAbwBmAHQAIABkAG8AZQBzACAAbgBvAHQAIAB3AGEAcgByAGEAbgB0ACAAbwByACAAYwBsAGEAaQBtACAAdABoAGEAdAAgAHQAaABlACAAaQBuAGYAbwByAG0AYQB0AGkAbwBuACAAZABpAHMAcABsAGEAeQBlAGQAIABpAG4AIAB0AGgAaQBzACAAYwBlAHIAdABpAGYAaQBjAGEAdABlACAAaQBzACAAYwB1AHIAcgBlAG4AdAAgAG8AcgAgAGEAYwBjAHUAcgBhAHQAZQAsACAAbgBvAHIAIABkAG8AZQBzACAAaQB0ACAAbQBhAGsAZQAgAGEAbgB5ACAAZgBvAHIAbQBhAGwAIABzAHQAYQB0AGUAbQBlAG4AdABzACAAYQBiAG8AdQB0ACAAdABoAGUAIABxAHUAYQBsAGkAdAB5ACAAbwByACAAcwBhAGYAZQB0AHkAIABvAGYAIABkAGEAdABhACAAcwBpAGcAbgBlAGQAIAB3AGkAdABoACAAdABoAGUAIABjAG8AcgByAGUAcwBwAG8AbgBkAGkAbgBnACAAcAByAGkAdgBhAHQAZQAgAGsAZQB5AC4AIDALBgNVHQ8EBAMCB4AwgaEGA1UdIwSBmTCBloAUdeBjdZAOPzN4/ah2f6tTCLPcC+qhcqRwMG4xCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpXYXNoaW5ndG9uMRAwDgYDVQQHEwdSZWRtb25kMR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xGDAWBgNVBAMTD01TTiBDb250ZW50IFBDQYIKYQlx2AABAAAABTBLBgNVHR8ERDBCMECgPqA8hjpodHRwOi8vY3JsLm1pY3Jvc29mdC5jb20vcGtpL2NybC9wcm9kdWN0cy9NU05Db250ZW50Q0EuY3JsME8GCCsGAQUFBwEBBEMwQTA/BggrBgEFBQcwAoYzaHR0cDovL3d3dy5taWNyb3NvZnQuY29tL3BraS9jZXJ0cy9NU05Db250ZW50Q0EuY3J0MA0GCSqGSIb3DQEBBQUAA4IBAQA6dVva4YeB983Ipos+zhzYfTAz4Rn1ZI7qHrNbtcXCCio/CrKeC7nDy/oLGbgCCn5wAYc4IEyQy6H+faXaeIM9nagqn6bkZHZTFiuomK1tN4V3rI8M23W8PvRqY4kQV5Qwfbz8TVhzEIdMG2ByoK7n9Fq0//kSLLoLqqPmC07oIcGNJPKDGxFzs/5FNEGyIybtmbIEeHSCJGKTDDAOnZAw6ji0873e2WIQsGBUm4VJN153xZgbnmdokWBfutkia6fnTUpcwofGolOe52fMYHYqaccxkP0vnmDGvloSPKOyXpc3RmI6g1rF7VzCQt290jG7A8+yb7OwM+rDooYMj4myMYIBkDCCAYwCAQEwgYowfDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCldhc2hpbmd0b24xEDAOBgNVBAcTB1JlZG1vbmQxHjAcBgNVBAoTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEmMCQGA1UEAxMdTVNOIENvbnRlbnQgQXV0aGVudGljYXRpb24gQ0ECCgmWGQYAAQAAANgwCQYFKw4DAhoFAKBdMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8XDTA2MDYyMzA4NTkzNVowIwYJKoZIhvcNAQkEMRYEFMni2bnV4P6Y9aUW5pzpPmz4hoU3MA0GCSqGSIb3DQEBAQUABIGApK4cGSUKvZiNT7GynJYEfIaSX/UuXf3wJF8cQd7AAy/ULnziD74KUgHfgqMr0h3U+dxbf14e/w6heQdf1Osq3Y+jNvPjhPqAAtIkcMRcgyYiOr973D6u7V5sbp6hKTa74bFVS5bg3ES55vBnAI58IL1JF5Y6qh64lRfhyYjmjjM=";
				session.sendWink("test1", k);
			} else if ("ink".equals(command)) {
				session.sendInk("ink.isf");
			} else if ("voice".equals(command)) {
				session.sendVoiceclip("voiceclip.wav");
			} else if ("file".equals(command)) {
				session.sendFile("1.txt");
			} else if ("p4".equals(command)) {
				session.sendActivity("http://www.xiaoi.com","INCE SP HOME PAGE");
			} else if ("webcam".equals(command)) {
				session.sendWebcam("192.168.1.26", 81, 101, 9001);
			} else if ("name".equals(command)) {
				if(param == null) param = "name-"+ new Random().nextInt(1000);
				server.setDisplayName(session.getRobot(), param);
			} else if ("psm".equals(command)) {
				if(param == null) param = "psm-" + new Random().nextInt(1000);
				server.setPersonalMessage(session.getRobot(), param);
			} else if ("dp".equals(command)) {
				server.setDisplayPicture("__default.dat");
			} else if ("ddp".equals(command)) {
				server.setDisplayPictureEx("__default.dat", "123.cab");
			} else if ("scene".equals(command)) {
				server.setScene("dota.jpg");
			} else if ("color".equals(command)) {
				server.setColorScheme(0xFF0000);
			} else if ("invite".equals(command)) {
				if (param != null) {
					session.inviteUser(param);
				}
			} else if ("fl".equals(command)) {
				server.requestContactList(session.getRobot());
			} else if ("push".equals(command)) {
				if (param != null) {
					server.pushMessage(session.getRobot(), param, "hello");
				}
			} else if ("create".equals(command)) {
				server.createSession(session.getRobot(), "boyce_lee@hotmail.com");
			} else if ("close".equals(command)) {
				session.close();
			} else {
				String text = "fontname: " + message.getFontName() + "\r";
				text = text + "fontstyle: " + message.getFontStyle() + "\r";
				String hex = Integer.toHexString((0xFFFFFF & message.getFontColor()));
				for(int k=hex.length(); k<6; k++) hex="0"+hex;
				text = text + "fontcolor: 0x" + hex + "\r";
				text = text + "message: " + message.getString() + "\r";
				session.send(text);
			}
		} catch (RobotException e) {
			e.printStackTrace();
		}
	}

	public void typingReceived(RobotSession session) {
		debugEvent();
	}

	public void nudgeReceived(RobotSession session) {
		debugEvent();
	}

	public void inkReceived(RobotSession session, byte[] ink) {
		try {
			debugEvent("size=" + ink.length);
			session.sendInk(ink);
		} catch (RobotException e) {
			e.printStackTrace();
		}
	}

	public void winkReceived(RobotSession session, RobotResource resource) {
		debugEvent("name=" + resource.getName() + ",size=" + resource.getSize());
	}

	public void voiceclipReceived(RobotSession session, RobotResource resource) {
		debugEvent("name=" + resource.getName() + ",size=" + resource.getSize());
	}

	public void activityAccepted(RobotSession session) {
		debugEvent();
	}

	public void activityRejected(RobotSession session) {
		debugEvent();
	}

	public void activityClosed(RobotSession session) {
		debugEvent();
	}

	public void activityLoaded(RobotSession session) {
		debugEvent();
	}

	public void activityReceived(RobotSession session, String data) {
		debugEvent("data=" + data);
	}

	public void fileInvited(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		debugEvent("name=" + fileDescriptor.getName() + ",size="
				+ fileDescriptor.getSize());
		try {
			session.sendFileRejection(fileDescriptor.getTransferId());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void fileAccepted(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		debugEvent("name=" + fileDescriptor.getName() + ",size="
				+ fileDescriptor.getSize());
	}

	public void fileRejected(RobotSession session,
			RobotFileDescriptor fileDescriptor) {
		debugEvent("name=" + fileDescriptor.getName() + ",size="
				+ fileDescriptor.getSize());
	}

	public void fileTransferEnded(RobotSession session ,
			RobotFileDescriptor fileDescriptor) {
		debugEvent("name=" + fileDescriptor.getName() + ",size="
				+ fileDescriptor.getSize());
	}

	public void fileTransferCancelled(RobotSession session ,
			RobotFileDescriptor fileDescriptor) {
		debugEvent("name=" + fileDescriptor.getName() + ",size="
				+ fileDescriptor.getSize());
	}

	public void fileTransferError(RobotSession session ,
			RobotFileDescriptor fileDescriptor) {
		debugEvent("name=" + fileDescriptor.getName() + ",size="
				+ fileDescriptor.getSize());
	}

	public void userJoined(RobotSession session, RobotUser user) {
		debugEvent();
	}

	public void userLeft(RobotSession session, RobotUser user) {
		debugEvent();
	}

	public void webcamAccepted(RobotSession session) {
		debugEvent();
	}

	public void webcamRejected(RobotSession session) {
		debugEvent();
	}

	public void userAdd(String robot, String user) {
		debugEvent("robot=" + robot + ",user=" + user);
	}

	public void userRemove(String robot, String user) {
		debugEvent("robot=" + robot + ",user=" + user);
	}

	public void userUpdated(String robot, RobotUser user) {
		debugEvent("robot=" + robot + ",friendly=" + user.getFriendlyName()
				+ ",status=" + user.getStatus());
	}

	public void personalMessageUpdated(String robot, String user,
			String personalMessage) {
		debugEvent("robot=" + robot + ",user=" + user + ",psm="
				+ personalMessage);

	}

	public void displayPictureUpdated(String robot, String user,
			RobotResource resource) {
		debugEvent("robot=" + robot + ",user=" + user + ",resname="
				+ resource.getName());
	}

	public void sceneUpdated(String robot, String user, RobotResource resource) {
		debugEvent("robot=" + robot + ",user=" + user + ",resname="
				+ resource.getName());
	}

	public void colorSchemeUpdated(String robot, String user, int colorScheme) {
		debugEvent("robot=" + robot + ",user=" + user + ",colorScheme="
				+ colorScheme);
	}

	public void contactListReceived(String robot, List<RobotUser> friendList) {
		debugEvent("robot=" + robot + ", size=" + friendList.size() + ")");
		for (RobotUser u : friendList) {
			System.out.println(u);
		}
	}

	
	public void exceptionCaught(RobotSession session, Throwable cause) {
		debugEvent(cause.getMessage());
		cause.printStackTrace();
	}

	private void debugEvent() {
		debugEvent(null);
	}
	private void debugEvent(String info) {

		StackTraceElement[] els = new Throwable().getStackTrace();
		if (info != null)
			System.out.println("[EVENT:" + els[1].getMethodName() + "] " + info);
		else
			System.out.println("[EVENT:" + els[2].getMethodName() + "]");
	}



}