//////////////////////////////////////////////////////////////////////////
/// @file       ComInterface.h
/// @brief      BotPlatform SDK Com Interface
//////////////////////////////////////////////////////////////////////////


/// IRobotServerFactory Interface
struct IRobotServerFactory : public IUnknown
{
    /// Init robot server factory
    /// @param[in]  threadCount Count of work threads
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall Init( LONG threadCount ) = 0;
    
    /// Destroy robot server factory    
    /// @return     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall Destroy( void ) = 0;
    
    /// Create a RobotServer instance
    /// @param[in]      ip              IP of BOTPLATFORM for RobotServer to login
    /// @param[in]      port            Port of BOTPLATFORM for RobotServer to login
    /// @param[out]     ppRobotServer   A pointer that receives a pointer to a new instance of the IRobotServer
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall CreateRobotServer( BSTR ip, LONG port, IRobotServer **ppRobotServer ) = 0;
};


/// IRobotServer Interface
struct IRobotServer : public IUnknown
{
    /// Calling this function, Robot Server will login BOTPLATFORM. It's a synchronous operation.
    /// @param[in]  spid    The unique ID assigned by INCESOFT
    /// @param[in]  sppwd   The password assigned by INCESOFT
    /// @param[in]  timeout Login timeout, the unit is millisecond. If the value less than or equal to 0, use default 1 minute
    /// @return             Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall Login( BSTR spid, BSTR sppwd, LONG timeout ) = 0;

    /// Robot server logout BOTPLATFORM
    /// @return             Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall Logout( void ) = 0;
    
    /// Set robot's display name
    /// @param[in]  robotAccount    specify a robot account to be modified, NULL for all robots
    /// @param[in]  displayName     a preferred display name
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SetDisplayName( BSTR robotAccount, BSTR displayName ) = 0;
    
    /// Set robot's personal message 
    /// @param[in]  robotAccount    specify a robot account to be modified, NULL for all robots
    /// @param[in]  personalMessage a preferred personal message
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SetPersonalMessage( BSTR robotAccount, BSTR personalMessage ) = 0;
    
    /// Set robot's display picture 
    /// @param[in]  robotAccount    specify a robot account to be modified, NULL for all robots
    /// @param[in]  displayPicture  unique name of the dp resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SetDisplayPicture( BSTR robotAccount, BSTR displayPicture ) = 0;
    
    /// Set robot's display picture 
    /// @param[in]  robotAccount    specify a robot account to be modified, NULL for all robots
    /// @param[in]  displayPicture  unique name of the dp resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
    /// @param[in]  largePicture    unique name of the ddp resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM 
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SetDisplayPictureEx( BSTR robotAccount, BSTR displayPicture, BSTR largePicture ) = 0;
    
    /// Set robot's scene
    /// @param[in]  robotAccount    specify a robot account to be modified, NULL for all robots
    /// @param[in]  scene           uri of the scene
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SetScene( BSTR robotAccount, BSTR scene ) = 0;
    
    /// Set robot's color scheme
    /// @param[in]  robotAccount    specify a robot account to be modified, NULL for all robots
    /// @param[in]  colorScheme     a preferred color
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SetColorScheme( BSTR robotAccount, LONG colorScheme ) = 0;
    
    /// Create a robot session.
    /// @param[in]  strRobot    robot id
    /// @param[in]  user        user id 
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall CreateSession( BSTR strRobot, BSTR user ) = 0;
    
    /// Push message to user. may be msn offline message,email message, etc. 
    /// @param[in]  strRobot    robot id
    /// @param[in]  user        user id
    /// @param[in]  message     message
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall PushMessage( BSTR strRobot, BSTR user, BSTR message ) = 0;
    
    /// Get the contact list of the specified robot.
    /// @param[in]  strRobot    robot id
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall RequestContactList( BSTR strRobot ) = 0;
    
    /// Get the contact list of the specified robot.
    /// @param[in]  strRobot    robot id
    /// @param[in]  user        user id
    /// @param[in]  resource    the resource to get
    /// @param[in]  saveUrl     a url receives a HTTP POST request which contains the resource data
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall RequestResource( BSTR strRobot, BSTR user, IRobotResource *resource, BSTR saveUrl ) = 0;
};


/// IRobotResource Interface
struct IRobotResource : public IUnknown
{
    /// Get the name of the resource
    /// @param[out]  pVal       Name
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Name( BSTR *pVal ) = 0;
    
    /// Get the digest of the resource
    /// @param[out]  pVal       Digest
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Digest( BSTR *pVal ) = 0;
    
    /// Get the size of the resource
    /// @param[out]  pVal       Size
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Size( LONG *pVal ) = 0;
};


/// IRobotUser Interface
struct IRobotUser : public IUnknown
{
    /// Get user's identifier (MSN account) 
    /// @param[out]  pVal       User's identifier
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_ID( BSTR *pVal ) = 0;
    
    /// Get user's friendly name
    /// @param[out]  pVal       User's friendly name
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_FriendlyName( BSTR *pVal ) = 0;
    
    /// Get user's status
    /// @param[out]  pVal       User's status \n NLN - online \n FLN - offline \n HDN - hidden \n BSY - busy \n IDL - idle \n BRB - be right back \n AWY - away \n PHN - on the phone \n LUN - out to lunch
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Status( BSTR *pVal ) = 0;

    /// Get user's client capabilities
    /// @param[out]  pVal       User's client capabilities \n 0x00000001 - User is online via Mobile \n 0x00000002 - User online with the MSN client \n 0x00000004 - User online GIF/ink awareness \n 0x00000008 - User online ISF/ink awareness \n 0x00000010 - There is a web camera on user's machine \n 0x00000020 - Support of message chunking (to allow larger messages over SB) \n 0x00000040 - Mobile IMs permitted flag \n 0x00000080 - Direct Watch enabled flag \n 0x00000200 - The user is online via the web client \n 0x00000400 - The user is a mobile buddy.The flag can be thought of as the Mobile buddy bit which indicates that the buddy is on a mobile device and the mobile properties \n 0x00000800 - The user is a federated client.The flag can be thought of as the Federated client bit which indicates that the buddy is connected on a client that does not connect directly to the MSN Messenger service \n 0x00001000 - The user has its HasSpace bit set \n 0x00004000 - The user supports Peer-to-Peer IM \n 0x00008000 - The user supports Winks \n 0x10000000 - Client version 6.0 \n 0x20000000 - Client version 6.1 \n 0x30000000 - Client version 6.2 \n 0x40000000 - Client version 7.0 
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_ClientID( LONG *pVal ) = 0;
};


/// IRobotSession Interface
struct IRobotSession : public IUnknown
{
    /// Create a message
    /// @param[out]     message     A pointer that receives a pointer to a new instance of the IRobotMessage
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall CreateMessage( IRobotMessage **message ) = 0;
    
    /// Send plain text message
    /// @param[in]     message      message
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendText( BSTR message ) = 0;
    
    /// Send RobotMessage
    /// @param[in]     message      RobotMessage
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendIM( IRobotMessage *message ) = 0;
    
    /// Send nudge
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendNudge( void ) = 0;
    
    /// Show the content of network resource in MSN Activity window
    /// @param[in]     url              network resource to be shown (generally are webpages or pictures)
    /// @param[in]     strFriendlyName  friendly name which represents the network resource in the activity invitation, can be NULL.
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendActivity( BSTR url, BSTR strFriendlyName ) = 0;
    
    /// Show the content of network resource in MSN Activity window
    /// @param[in]     appid        id of the MSN Activity
    /// @param[in]     appname      name of the MSN Activity
    /// @param[in]     data         network resource to be shown (generally are webpages or pictures) 
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendActivityEx( BSTR appid, BSTR appname, BSTR data ) = 0;
    
    /// Check whether the activity window opened
    /// @param[out]     pVal    ActivityOpened
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_ActivityOpened( VARIANT_BOOL *pVal ) = 0;
    
    /// Send a typing infomation which will be presented on the bottom of the conversation panel
    /// @return                 Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendTyping( void ) = 0;
    
    /// Get a user in session
    /// @param[in]      userid      user id
    /// @param[out]     ppUser      user
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall GetUser( BSTR userid, IRobotUser **ppUser ) = 0;
    
    /// Get a users' collection in session 
    /// @param[out]     ppUsers     users
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Users( IRobotUsers **ppUsers ) = 0;
    
    /// Get the robot account(MSN Passport) associated with this session
    /// @param[out]     pStr        robot account
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Robot( BSTR *pStr ) = 0;
    
    /// Get the open mode of session
    /// @param[out]     pVal        0 - Unknown \n 1 - Indicates that user opens a conversation window \n 2 - Indicates that session timeout and then reconnects
    /// @return                     Returns S_OK if successful, or an error value otherwise
    /// @see OPEN_MODE
    virtual HRESULT __stdcall get_OpenMode( LONG *pVal ) = 0;
    
    /// Check whether the session is closed. If session is closed, you cannot send any messages.
    /// @param[out]     pVal        closed
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Closed( VARIANT_BOOL *pVal ) = 0;
    
    /// Close this session.
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall Close( void ) = 0;
    
    /// Invite a user into the current conversation.
    /// @param[in]     user         user
    /// @return                     Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall InviteUser( BSTR user ) = 0;
    
    /// Send a file
    /// @param[in]     uri              unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
    /// @param[in]     strFriendlyName  friendly name which represents the file in the file transfer invitation, can be NULL
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendFile( BSTR uri, BSTR strFriendlyName ) = 0;
    
    /// Accept a file transfer request, and a url should be specified for saving the file data.
    /// @param[in]     transferId       id of the current file transfer.
    /// @param[in]     saveUrl          a url to receive a HTTP POST request to get the data of the file.
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendFileAcceptance( BSTR transferId, BSTR saveUrl ) = 0;
    
    /// Reject a file transfer request.
    /// @param[in]     transferId       id of the current file transfer.
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendFileRejection( BSTR transferId ) = 0;
    
    /// Cancel a file transfer.
    /// @param[in]     transferId       id of the current file transfer.
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendFileCancellation( BSTR transferId ) = 0;
    
    /// Send a ink message
    /// @param[in]     inkData          data of the ink message. 
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendInk( BSTR inkData ) = 0;
    
    /// Send a msn wink
    /// @param[in]     uri              unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
    /// @param[in]     stamp            base64 encoded signature of the file.It is a S/MIME signature of the Base64 encoded hash of the Content. If this parameter is NULL, wink is default.
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendWink( BSTR uri, BSTR stamp ) = 0;
    
    /// Send a voice clip
    /// @param[in]     uri              unique name of the resource pre-uploaded to BOTPLATFORM and verified by BOTPLATFORM
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendVoiceclip( BSTR uri ) = 0;
    
    /// Send webcam Once the connection is established, the client that connected to the other peer should authenticate himself with sending the message (no header, no nothing) : "recipientid=X&sessionid=Y\r\n\r\n" The other peer should then send (if correctly authenticated) the message "connected\r\n\r\n" The connecting peer should also send "connected\r\n\r\n" From this point the connection is done and The connecting peer is authenticated Subsequently, the server may send the encoded data of the webcam in mimic format 
    /// @param[in]     serverIP         webcam resource server IP address
    /// @param[in]     serverPort       webcam resource server port
    /// @param[in]     recipientid      the authentication info, should be between 100 and 199
    /// @param[in]     sessionid        the authentication info, should be between 9000 and 9999
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall SendWebcam( BSTR serverIP, LONG serverPort, LONG recipientid, LONG sessionid ) = 0;
};


/// IRobotUsers Interface
struct IRobotUsers : public IUnknown
{
    /// Get user by index
    /// @param[in]     idx              idx
    /// @param[out]    ppUser           user
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall Item( LONG idx, IRobotUser **ppUser ) = 0;
    
    /// Get count of users
    /// @param[out]     pVal            count of users
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall Count( LONG *pVal ) = 0;
};


/// IRobotMessage Interface
struct IRobotMessage : public IUnknown
{
    /// Get robot message preface. eg: xxx says.
    /// @param[out]    pVal             Signature
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Signature( BSTR *pVal ) = 0;
    
    /// Set robot message preface
    /// @param[in]    newVal            Signature
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall put_Signature( BSTR newVal ) = 0;
    
    /// Get font style of the message, as bold, italic, strikeout, underline
    /// @param[out]    pVal             FontStyle
    /// @return                         Returns S_OK if successful, or an error value otherwise
    /// @see FONT_STYLE
    virtual HRESULT __stdcall get_FontStyle( LONG *pVal ) = 0;
    
    /// Set font style of the message, as bold, italic,strikeout, underline
    /// @param[in]    newVal            FontStyle
    /// @return                         Returns S_OK if successful, or an error value otherwise
    /// @see FONT_STYLE
    virtual HRESULT __stdcall put_FontStyle( LONG newVal ) = 0;
    
    /// Get font name of the message
    /// @param[out]    pVal             FontName
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_FontName( BSTR *pVal ) = 0;
    
    /// Set font name of the message
    /// @param[in]    newVal            FontName
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall put_FontName( BSTR newVal ) = 0;
    
    /// Get font color of the message
    /// @param[out]    pVal             FontColor
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_FontColor( LONG *pVal ) = 0;
    
    /// Set font color of the message
    /// @param[in]    newVal            FontColor
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall put_FontColor( LONG newVal ) = 0;
    
    /// Get the plain text of the message
    /// @param[out]    pVal             Text
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Text( BSTR *pVal ) = 0;
    
    /// Set the plain text of the message
    /// @param[in]    newVal            Text
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall put_Text( BSTR newVal ) = 0;
    
    /// Register custom emoticon
    /// @param[in]    shortcut          The shortcut of the emoticon. If shortcut appears in message, it'll show as an icon corresponding to your 'filename'.
    /// @param[in]    filename          The filename of emotion. Submitted to INCE by SP, and a certified file of INCE
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall RegisterEmoticon( BSTR shortcut, BSTR filename ) = 0;
    
    /// Deregister the emoticon After the invocation of this function, the shortcut appearing in the message will show as plain text.
    /// @param[in]    shortcut          The shortcut of the emoticon. If shortcut appears in message, it'll show as an icon corresponding to your 'filename'.
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual  HRESULT __stdcall DeregisterEmoticon( BSTR shortcut ) = 0;
};


/// IRobotFileDescriptor Interface
struct IRobotFileDescriptor : public IUnknown
{
    /// Get id of the current transfer.
    /// @param[out]    pVal             TransferId
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_TransferId( BSTR *pVal ) = 0;
    
    /// Get name of the current transfer.
    /// @param[out]    pVal             Name
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Name( BSTR *pVal ) = 0;
    
    /// Get size of the current transfer.
    /// @param[out]    pVal             Size
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Size( LONG *pVal ) = 0;
    
    /// Get thumbnail of the current transfer (if available)
    /// @param[out]    pVal             Thumbnail
    /// @return                         Returns S_OK if successful, or an error value otherwise
    virtual HRESULT __stdcall get_Thumbnail( BSTR *pVal ) = 0;
};


/// OPEN_MODE enum
enum OPEN_MODE
{
    OPEN_MODE_UNKONW    = 0, ///<- A flag indicates that a new conversation started by unkonw reason.
    OPEN_MODE_CONV_OPEN = 1, ///<- A flag indicates that a new conversation started by user.
    OPEN_MODE_TIMEOUT   = 2, ///<- A flag indicates that a new conversation started by reconnecting after session timeout.
    OPEN_MODE_ROBOT     = 3  ///<- A flag indicates that a new conversation started by robot.
};


/// FONT_STYLE enum
enum FONT_STYLE
{
    STYLE_BOLD          = 1, ///<- Font style - bold
    STYLE_ITALIC        = 2, ///<- Font style - italic
    STYLE_UNDERLINE     = 4, ///<- Font style - underline
    STYLE_STRIKE        = 8, ///<- Font style - strikeout
    STYLE_BOLD_ITALIC   = 3, ///<- Font style - bold + underline
    STYLE_ALL           = 15 ///<- Font style - bold + italic + strikeout + underline
};

/// _IRobotServerEvents interface
struct _IRobotServerEvents : public IDispatch
{
    HRESULT SessionOpened (
        struct IRobotSession * session );
    HRESULT SessionClosed (
        struct IRobotSession * session );
    HRESULT MessageReceived (
        struct IRobotSession * session,
        struct IRobotMessage * message );
    HRESULT NudgeReceived (
        struct IRobotSession * session );
    HRESULT ActivityAccepted (
        struct IRobotSession * session );
    HRESULT ActivityRejected (
        struct IRobotSession * session );
    HRESULT ActivityReceived (
        struct IRobotSession * session,
        _bstr_t data );
    HRESULT ActivityLoaded (
        struct IRobotSession * session );
    HRESULT ActivityClosed (
        struct IRobotSession * session );
    HRESULT UserJoined (
        struct IRobotSession * session,
        struct IRobotUser * user );
    HRESULT UserLeft (
        struct IRobotSession * session,
        struct IRobotUser * user );
    HRESULT UserAdd (
        _bstr_t strRobot,
        _bstr_t user );
    HRESULT UserRemove (
        _bstr_t strRobot,
        _bstr_t user );
    HRESULT ExceptionCaught (
        struct IRobotSession * session,
        long cause );
    HRESULT FileAccepted (
        struct IRobotSession * session,
        struct IRobotFileDescriptor * fileDescriptor );
    HRESULT FileRejected (
        struct IRobotSession * session,
        struct IRobotFileDescriptor * fileDescriptor );
    HRESULT FileTransferEnded (
        struct IRobotSession * session,
        struct IRobotFileDescriptor * fileDescriptor );
    HRESULT FileTransferCancelled (
        struct IRobotSession * session,
        struct IRobotFileDescriptor * fileDescriptor );
    HRESULT FileTransferError (
        struct IRobotSession * session,
        struct IRobotFileDescriptor * fileDescriptor );
    HRESULT FileInvited (
        struct IRobotSession * session,
        struct IRobotFileDescriptor * fileDescriptor );
    HRESULT WebcamAccepted (
        struct IRobotSession * session );
    HRESULT WebcamRejected (
        struct IRobotSession * session );
    HRESULT WebcamClosed (
        struct IRobotSession * session );
    HRESULT WebcamError (
        struct IRobotSession * session );
    HRESULT UserUpdated (
        _bstr_t strRobot,
        struct IRobotUser * user );
    HRESULT PersonalMessageUpdated (
        _bstr_t strRobot,
        _bstr_t user,
        _bstr_t personalMessage );
    HRESULT ContactListReceived (
        _bstr_t strRobot,
        struct IRobotUsers * contactList );
    HRESULT InkReceived (
        struct IRobotSession * session,
        _bstr_t ink );
    HRESULT WinkReceived (
        struct IRobotSession * session,
        struct IRobotResource * resource );
    HRESULT VoiceclipReceived (
        struct IRobotSession * session,
        struct IRobotResource * resource );
    HRESULT TypingReceived (
        struct IRobotSession * session );
    HRESULT SceneUpdated (
        _bstr_t strRobot,
        _bstr_t user,
        struct IRobotResource * resource );
    HRESULT DisplayPictureUpdated (
        _bstr_t strRobot,
        _bstr_t user,
        struct IRobotResource * resource );
    HRESULT ColorSchemeUpdated (
        _bstr_t strRobot,
        _bstr_t user,
        long colorScheme );
};




   
