In this page, let's get a better understanding about SessionOpenEvent.

The SessionOpenEvent will be fired under the circumstances shown blow:
  * When your robot wants to begin a chat with a user (which means to call the 'createSession' method in RobotServer),the created session's OpenMode is 3.
  * When a user opens a chat window, there is nothing connects to the server. Thus, opening a chat window has nothing to do with receiving a SessionOpenEvent. When the user performs a certain action in the chat window, the SessionOpenEvent will be fired before the user's action takes effect. At the moment, the created session's OpenMode is 1.
  * When your robot disconnects to our platform, you may consider all the sessions as closed. When your robot reconnects, all the users interacting with your robot will cause the SessionOpenEvent fires again (to guarantee that an opened session must has a corresponding close, they should always appear in couple).The OpenMode is 2 at the moment.

To avoid your business logic written for the SessionOpenEvent has a nagative effect(as the logic performs frequently, your users may get annoyed), we suggest you to take some measures to store the user's information when the SessionOpenEvent is fired for the first time.

eg: You want your bot to simply say 'hello' to a user when the SessionOpenEvent is fired, just save the current user's id and make it expired after 24 hours, then the current user won't receive any 'hello' in a day.