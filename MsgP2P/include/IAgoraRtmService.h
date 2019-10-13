//
//  The Agora RTM SDK
//
//
//  Copyright (c) 2019 Agora.io. All rights reserved.
//
#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define AGORA_CALL __cdecl
#if defined(AGORARTC_EXPORT)
#define AGORA_API extern "C" __declspec(dllexport)
#else
#define AGORA_API extern "C" __declspec(dllimport)
#endif
#define _AGORA_CPP_API

#elif defined(__APPLE__)
#define AGORA_API __attribute__((visibility("default"))) extern "C"
#define AGORA_CALL
#define _AGORA_CPP_API

#elif defined(__ANDROID__) || defined(__linux__)
#if defined(__ANDROID__) && defined(FEATURE_RTM_STANDALONE_SDK)
#define AGORA_API extern "C"
#define _AGORA_CPP_API
#else
#define AGORA_API extern "C" __attribute__((visibility("default")))
#define _AGORA_CPP_API __attribute__((visibility("default")))
#endif
#define AGORA_CALL

#else
#define AGORA_API extern "C"
#define AGORA_CALL
#define _AGORA_CPP_API
#endif

namespace agora {
  namespace rtm {
      
      /**
       @brief Error codes related to Initialization.
       */
      enum INIT_ERR_CODE {
          
          /**
           0: Initialization succeeds. No error occurs.
           */
          INIT_ERR_OK = 0,
          
          /**
           1: A common failure occurs during initialization.
           */
          INIT_ERR_FAILURE = 1,
          
          /**
           2: The SDK is already initialized.
           */
          INIT_ERR_ALREADY_INITIALIZED = 2,
          
          /**
           3: The App ID is invalid.
           */
          INIT_ERR_INVALID_APP_ID = 3,
          
          /**
           4: The event handler is empty.
           */
          INIT_ERR_INVALID_ARGUMENT = 4,
      };
     
      
    /**
     @brief Error codes related to login.
     */
    enum LOGIN_ERR_CODE {
      
      /**
       0: Login succeeds. No error occurs.
       */
      LOGIN_ERR_OK = 0,
        
      /**
       1: Login fails. The reason is unknown.
       */
      LOGIN_ERR_UNKNOWN = 1,
      
      /**
       2: Login is rejected by the server.
       */
      LOGIN_ERR_REJECTED = 2, // Occurs when not initialized or server reject
        
      /**
       3: Invalid login argument.
       */
      LOGIN_ERR_INVALID_ARGUMENT = 3,
        
      /**
       4: The App ID is invalid.
       */
      LOGIN_ERR_INVALID_APP_ID = 4,
        
      /**
       5: Invalid token.
       */
      LOGIN_ERR_INVALID_TOKEN = 5,
       
      /**
       6: The token has expired, and hence login is rejected.
       */
      LOGIN_ERR_TOKEN_EXPIRED = 6,
       
      /**
       7: Unauthorized login.
       */
      LOGIN_ERR_NOT_AUTHORIZED = 7,
       
      /**
       8: The user has already logged in or is logging in the Agora RTM system.
       */
      LOGIN_ERR_ALREADY_LOGGED_IN = 8,
       
      /**
       9: The login times out.
       */
      LOGIN_ERR_TIMEOUT = 9,
       
      /**
       10: The call frequency of the \ref agora::rtm::IRtmService::login "login" method exceeds the limit of two queries per second.
       */
      LOGIN_ERR_TOO_OFTEN = 10,
        
      /**
       101: The SDK is not initialized.
      */
      LOGIN_ERR_NOT_INITIALIZED = 101,
    };
      
    /**
     @brief Error codes related to logout.
     */
    enum LOGOUT_ERR_CODE {
       
      /**
       0: Logout succeeds. No error occurs.
       */
      LOGOUT_ERR_OK = 0,
      
      /**
       1: Logout fails, possibly because the SDK is not initialized or because the user has not logged in.
       */
      LOGOUT_ERR_REJECTED = 1,
        
      /**
       101: The SDK is not initialized.
       */
      LOGOUT_ERR_NOT_INITIALIZED = 101,
        
      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the user logs out of the Agora RTM system.
       */
      LOGOUT_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to renewing the token.
     */
    enum RENEW_TOKEN_ERR_CODE {
        
      /**
       0: The method call succeeds.
       */
      RENEW_TOKEN_ERR_OK = 0,
        
      /**
       1: The method call fails.
       */
      RENEW_TOKEN_ERR_FAILURE = 1,
 
      /**
       2: The method call fails. The argument is invalid. 
       */
      RENEW_TOKEN_ERR_INVALID_ARGUMENT = 2,
        
      /**
       3: The Agora RTM service is not initialized.
       */
      RENEW_TOKEN_ERR_REJECTED = 3,
 
      /**
       4: The method call frequency exceeds the limit of two queries per second.
       */
      RENEW_TOKEN_ERR_TOO_OFTEN = 4,
  
      /**
       5: The token has expired.
       */
      RENEW_TOKEN_ERR_TOKEN_EXPIRED = 5,
  
      /**
       6: The token is invalid.
       */
      RENEW_TOKEN_ERR_INVALID_TOKEN = 6,
         
      /**
       101: The SDK is not initialized.
       */
      RENEW_TOKEN_ERR_NOT_INITIALIZED = 101,
        
      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before renewing the token.
       */
      RENEW_TOKEN_ERR_USER_NOT_LOGGED_IN = 102,
    };
 
    /**
     @brief Connection states between the SDK and the Agora RTM system.
     */
    enum CONNECTION_STATE {
        
      /**
       1: The initial state. The SDK is disconnected from the Agora RTM system.
       
       When the user calls the \ref agora::rtm::IRtmService::login "login" method, the SDK starts to log in the Agora RTM system, triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback, and switches to the \ref agora::rtm::CONNECTION_STATE_CONNECTING "CONNECTION_STATE_CONNECTING" state.
       */
      CONNECTION_STATE_DISCONNECTED = 1,
        
      /**
       2: The SDK is logging in the Agora RTM system.

       - If the user successfully logs in the Agora RTM system and receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginSuccess "onLoginSuccess" callback, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_CONNECTED "CONNECTION_STATE_CONNECTED" state.
       - If the user fails to login the Agora RTM system and receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginFailure "onLoginFailure" callback, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_DISCONNECTED "CONNECTION_STATE_DISCONNECTED" state.
       */
      CONNECTION_STATE_CONNECTING = 2,
        
      /**
       3: The SDK has logged in the Agora RTM system.

       - If the connection state between the SDK and the Agora RTM system is interrupted because of network issues, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       - If the login is banned by the server because, for example, another instance logs in the Agora RTM system with the same user ID, the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback is triggered, and the SDK is switched to the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" state.
       - If the user calls the \ref agora::rtm::IRtmService::logout "logout" method to log out of the Agora RTM system and receives the \ref agora::rtm::IRtmServiceEventHandler::onLogout "onLogout" callback (error code = `LOGOUT_ERR_OK`), the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_DISCONNECTED "CONNECTION_STATE_DISCONNECTED" state.
       */
      CONNECTION_STATE_CONNECTED = 3,
        
      /**
       4: The connection state between the SDK and the Agora RTM system is interrupted due to network issues, and the SDK keeps re-logging in the Agora RTM system.

       - If the SDK successfully re-logs in the Agora RTM system, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_CONNECTED "CONNECTION_STATE_CONNECTED" state. The SDK automatically adds the user back to the channel(s) he or she was in when the connection was interrupted, and synchronizes the local user's attributes with the server. 
       - If the SDK cannot re-log in the Agora RTM system, it stays in the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state and keeps re-logging in the system.
       */
      CONNECTION_STATE_RECONNECTING = 4,
        
      /**
       5: The SDK gives up logging in the Agora RTM system, mainly because another instance has logged in the Agora RTM system with the same user ID.
       
       Call the \ref agora::rtm::IRtmService::logout "logout" method before calling the \ref agora::rtm::IRtmService::login "login" method again.
       */
      CONNECTION_STATE_ABORTED = 5,
    };

    /**
     @brief Reasons for a connection state change.
     */
    enum CONNECTION_CHANGE_REASON {
        
      /**
       1: The SDK is logging in the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGIN = 1,
        
      /**
       2: The SDK has logged in the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGIN_SUCCESS = 2,
        
      /**
       3: The SDK fails to log in the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGIN_FAILURE = 3,
        
      /**
       4: The SDK fails to log in the Agora RTM system within six seconds and gives up.
       */
      CONNECTION_CHANGE_REASON_LOGIN_TIMEOUT = 4,
        
      /**
       5: The connection between the SDK and the Agora RTM system is interrupted. The system defines an interruption when the SDK loses connection with the Agora RTM system for network reasons and cannot recover in four seconds.
       */
      CONNECTION_CHANGE_REASON_INTERRUPTED = 5,
        
      /**
       6: The SDK has logged out of the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGOUT = 6,
        
      /**
       7: The SDK login to the Agora RTM system is banned by Agora.
       */
      CONNECTION_CHANGE_REASON_BANNED_BY_SERVER = 7,
        
      /**
       8: Another user is logging in the Agora RTM system with the same User ID.
       */
      CONNECTION_CHANGE_REASON_REMOTE_LOGIN = 8,
    };

    /**
     @brief Error codes related to sending a peer-to-peer message.
     */
    enum PEER_MESSAGE_ERR_CODE {
        
      /**
       0: The specified user receives the peer-to-peer message.
       */
      PEER_MESSAGE_ERR_OK = 0,

      /**
       1: The sender fails to send the peer-to-peer message. Reasons may include:
     
       - The connection between the SDK and the Agora RTM system is interrupted, and the SDK is in the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       - The sender does not call the \ref agora::rtm::IRtmService::login "login" method before sending out a peer-to-peer message.
       */
      PEER_MESSAGE_ERR_FAILURE = 1,
        
      /**
       2: A timeout occurs when sending the peer-to-peer message. The current timeout is set as five seconds.
       */
      PEER_MESSAGE_ERR_SENT_TIMEOUT = 2,
        
      /**
       3: The specified user is offline and does not receive this peer-to-peer message.
       */
      PEER_MESSAGE_ERR_PEER_UNREACHABLE = 3,
        
     /**
      4: The receiver is offline and does not receive this offline peer-to-peer message, but the server has cached it and will re-send it once he/she is back online.
      */
      PEER_MESSAGE_ERR_CACHED_BY_SERVER = 4,
        
     /**
      5: The method call frequency exceeds the limit of 60 queries per second (channel and peer messages taken together).
      */
      PEER_MESSAGE_ERR_TOO_OFTEN = 5,
        
     /**
      6: The user ID is invalid.
      */
      PEER_MESSAGE_ERR_INVALID_USERID = 6,
        
     /**
      7: The message is null or exceeds 32 KB in length.       
	  */
      PEER_MESSAGE_ERR_INVALID_MESSAGE = 7,
        
     /**
      101: The SDK is not initialized.
      */
      PEER_MESSAGE_ERR_NOT_INITIALIZED = 101,
    
     /**
      102: The sender does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before sending the peer-to-peer message.
      */
      PEER_MESSAGE_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to joining a channel.
     */
    enum JOIN_CHANNEL_ERR {
        
      /**
       0: The user joins the channel successfully.
       */
      JOIN_CHANNEL_ERR_OK = 0,

      /**
       1: The user fails to join the channel. Reasons may include:
     
       - The connection between the SDK and the system is interrupted, and the SDK is in the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       - The user does not call the \ref agora::rtm::IRtmService::login "login" method before joining the channel.
       - The method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the user joins the channel.
       - The number of the RTM channels you join exceeds the limit of 20.
       */
      JOIN_CHANNEL_ERR_FAILURE = 1,
        
      /**
       2: The user cannot join the channel, possibly because he/she is already in the channel.
       */
      JOIN_CHANNEL_ERR_REJECTED = 2, // Usually occurs when the user is already in the channel
        
      /**
       3: The user fails to join the channel; the argument is invalid.
       */
      JOIN_CHANNEL_ERR_INVALID_ARGUMENT = 3,
        
      /**
       4: A timeout occurs when joining the channel. The current timeout is set as five seconds.
       */
      JOIN_CHANNEL_TIMEOUT = 4,
        
     /**
      5: The number of the RTM channels you join exceeds 20.
      */
      JOIN_CHANNEL_ERR_EXCEED_LIMIT = 5,
        
     /**
      6: The user is joining or has joined the channel.
      */
      JOIN_CHANNEL_ERR_ALREADY_JOINED = 6,
        
      /**
      7: The method call frequency exceeds 50 queries every three seconds.
      */
      JOIN_CHANNEL_ERR_TOO_OFTEN = 7,
        
     /**
      101: The SDK is not initialized.
      */
      JOIN_CHANNEL_ERR_NOT_INITIALIZED = 101,
        
     /**
      102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the user joins the channel.
      */
      JOIN_CHANNEL_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to leaving a channel.
     */
    enum LEAVE_CHANNEL_ERR {
      
      /**
       0: The user leaves the channel successfully.
       */
      LEAVE_CHANNEL_ERR_OK = 0,
        
      /**
       1: The user fails to leave the channel.
       */
      LEAVE_CHANNEL_ERR_FAILURE = 1,
        
      /**
       2: The user cannot leave the channel, possibly because the user is not in the channel.
       */
      LEAVE_CHANNEL_ERR_REJECTED = 2,
        
      /**
       3: The user is not in the channel.
       */
      LEAVE_CHANNEL_ERR_NOT_IN_CHANNEL = 3,
        
      /**
       101: The SDK is not initialized.
       */
      LEAVE_CHANNEL_ERR_NOT_INITIALIZED = 101,
    
      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before calling the \ref agora::rtm::IChannel::leave "leave" method.
       */
      LEAVE_CHANNEL_ERR_USER_NOT_LOGGED_IN = 102,
    };
      
    /**
     @brief Reasons why a user leaves the channel.
     */
    enum LEAVE_CHANNEL_REASON {
        
      /**
       1: The user has quit the call.
       */
      LEAVE_CHANNEL_REASON_QUIT = 1,
        
      /**
       2: The user is banned by the server.
       */
      LEAVE_CHANNEL_REASON_KICKED = 2,
    };

    /**
     @brief Error codes related to sending a channel message.
     */
    enum CHANNEL_MESSAGE_ERR_CODE {
        
      /**
       0: The server receives the channel message.
       */
      CHANNEL_MESSAGE_ERR_OK = 0,

      /**
       1: The user fails to join the channel. Reasons may include:

       - The connection state between the SDK and th system is interrupted, and the SDK is in the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       - The sender does not call the \ref agora::rtm::IRtmService::login "login" method before sending out a channel message.
       */
      CHANNEL_MESSAGE_ERR_FAILURE = 1,
             
      /**
       2: A timeout occurs when sending the channel message. The current timeout is set as five seconds.
       */
      CHANNEL_MESSAGE_ERR_SENT_TIMEOUT = 2,
        
      /**
       3: The method call frequency exceeds the limit of 60 queries per second (channel and peer messages taken together).
       */
      CHANNEL_MESSAGE_ERR_TOO_OFTEN = 3,
        
      /**
       4: The message is null or exceeds 32 KB in length.
       */
      CHANNEL_MESSAGE_ERR_INVALID_MESSAGE = 4,
        
      /**
       101: The SDK is not initialized.
       */
      CHANNEL_MESSAGE_ERR_NOT_INITIALIZED = 101,
    
      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before sending out a channel message.
       */
      CHANNEL_MESSAGE_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     Error codes related to retrieving a member list.
     */
    enum GET_MEMBERS_ERR {
  
      /**
       0: The user successfully retrieves a member list of the channel.
       */
      GET_MEMBERS_ERR_OK = 0,
    
      /**
       1: The user fails to retrieve a member list of the channel.
       */
      GET_MEMBERS_ERR_FAILURE = 1,
        
      /**
       2: Cannot retrieve a member list of the channel.
       */
      GET_MEMBERS_ERR_REJECTED = 2,
        
      /**
       3: A timeout occurs when retreiving a member list of the channel. The current timeout is set as five seconds. 
       */
      GET_MEMBERS_ERR_TIMEOUT = 3,
        
      /**
       4: The method call frequency exceeds the limit of five queries every two seconds.
       */
      GET_MEMBERS_ERR_TOO_OFTEN = 4,
    
      /**
       5: The user is not in channel.
       */
      GET_MEMBERS_ERR_NOT_IN_CHANNEL = 5,
        
      /**
       101: The SDK is not initialized.
       */
      GET_MEMBERS_ERR_NOT_INITIALIZED = 101,
        
      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before retrieving a member list.
       */
      GET_MEMBERS_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     Error codes related to querying the online status of the specified peer(s).
     */
    enum QUERY_PEERS_ONLINE_STATUS_ERR {
      
      /**
       0: The method call succeeds.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_OK = 0,

      /**
       1: The method call fails.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_FAILURE = 1,
        
      /**
       2: The method call fails. The argument is invalid. 
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_INVALID_ARGUMENT = 2,
        
      /**
       3: The method call fails. The peer user has not logged in the Agora RTM system.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_REJECTED = 3,
        
      /**
       4: A timeout occurs when querying the online status. The current timeout is set as 10 seconds.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_TIMEOUT = 4,
        
      /**
       5: The method call frequency exceeds the limit of 10 queries every five seconds.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_TOO_OFTEN = 5,
        
      /**
       101: The SDK is not initialized.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_NOT_INITIALIZED = 101,
    
      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before querying the online status.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_USER_NOT_LOGGED_IN = 102,
    };
      
    /**
     Error codes related to attrubute operations.
     */
    enum ATTRIBUTE_OPERATION_ERR {
        
        /**
         0: The attribute operation succeeds.
         */
        ATTRIBUTE_OPERATION_ERR_OK = 0,
        
        /**
         1: The SDK is not ready for this attribute operation. Only after the \ref agora::rtm::IRtmService::login "login" method call succeeds can you call the attribute-related methods. 
         */
        ATTRIBUTE_OPERATION_ERR_NOT_READY = 1,
        
        /**
         2: The attribute operation fails.
         */
        ATTRIBUTE_OPERATION_ERR_FAILURE = 2,
        
        /**
         3: The argument is invalid.
         */
        ATTRIBUTE_OPERATION_ERR_INVALID_ARGUMENT = 3,
        
        /**
         4: The size of this attribute setting exceeds the limit of 16 KB.
         */
        ATTRIBUTE_OPERATION_ERR_SIZE_OVERFLOW = 4,
        
        /**
         5: The method call frequency exceeds the limit.
         
         - For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the call frequency limit is 10 queries every five seconds.
         - For \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" and \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" taken together, the call frequency limit is 40 queries every five seconds.
         */
        ATTRIBUTE_OPERATION_ERR_TOO_OFTEN = 5,
        
        /**
         6: The specified user is not found, either because the user is offline or because the user does not exist.
         */
        ATTRIBUTE_OPERATION_ERR_USER_NOT_FOUND = 6,
        
        /**
         7: A timeout occurs during the attribute operation. The current timeout is set as five seconds.
         */
        ATTRIBUTE_OPERATION_ERR_TIMEOUT = 7,
        
        /**
         101: The SDK is not initialized.
         */
        ATTRIBUTE_OPERATION_ERR_NOT_INITIALIZED = 101,
        
        /**
         102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the attribute operation.
         */
        ATTRIBUTE_OPERATION_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Message types.
     */
    enum MESSAGE_TYPE {
        
      /**
       0: The message type is undefined.
       */
      MESSAGE_TYPE_UNDEFINED = 0,

      /**
       1: A text message.
       */
      MESSAGE_TYPE_TEXT = 1,
    };
     
     /**
      @brief Message sending options.
      */
     struct SendMessageOptions{
         
         /**
          Enables offline messaging.
          
          - true: Enables offline messaging.
          - false: (default) Disables offline messaging.
          */
         bool enableOfflineMessaging;
         SendMessageOptions():enableOfflineMessaging(false){}
     };
   
    /**
     @brief A data structure holding an RTM attribute and its value.
     */
    struct RtmAttribute
    {
    
        /**
         Attribute name. Must be visible characters and not exceed 32 Bytes in length.
         */
        const char* key;
  
        /**
         Value of the attribute. Must not exceed 8 KB in length.
         */
        const char* value;
    };

      
      
    /**
     @brief The class for setting or retrieving attributes of a channel and peer-to-peer message.
     */
    class IMessage
    {
    protected:
      virtual ~IMessage() {}
    public:
        
      /**
       Retrieves the unique ID of the channel or peer-to-peer message.
       
       @return The message ID, which is created within the message.
       */
      virtual long long getMessageId() const = 0;
        
      /**
       Retrieves the channel or peer-to-peer message type.

       @return The message type. See #MESSAGE_TYPE. 
       */
      virtual MESSAGE_TYPE getMessageType() const = 0;
        
      /**
       Sets the channel or peer-to-peer text message.
       
       @param str The text message to be set. Must not exceed 32 KB in length. 
       */
      virtual void setText(const char *str) = 0;
        
      /**
       Retrieves the channel or peer-to-peer text message.
       
       @return The received text message.
       */
      virtual const char *getText() const = 0;
        
      /**
       Allows the receiver to retrieve the timestamp of when the messaging server receives this message.
       
       @note The returned timestamp is on a millisecond time-scale. It is for demonstration purposes only, not for strict ordering of messages.
       
       @return The timestamp (ms) of when the messaging server receives this message.
       */
      virtual long long getServerReceivedTs() const = 0;
        
      /**
       Allows the receiver to check whether this message has been cached on the server.
       
       @note This method returns false if a message is not cached by the server. Only if the sender sends the message as an offline message (sets \ref agora::rtm::SendMessageOptions::enableOfflineMessaging "enableOfflineMessaging" as true) when the specified user is offline, does the method return true when the user is back online.
       
       @return
       - true: This message has been cached on the server (the server caches this message and re-sends it to the receiver when he/she is back online).
       - false: This message has not been cached on the server.
       */
      virtual bool isOfflineMessage() const = 0;
        
      /**
       Releases all resources used by the IMessage instance.
       
       @note For the message receiver: please access and save the content of the IMessage instance when receiving the \ref agora::rtm::IChannelEventHandler::onMessageReceived "onMessageReceived" or the \ref agora::rtm::IRtmServiceEventHandler::onMessageReceivedFromPeer "onMessageReceivedFromPeer" callback. The SDK will release the IMessage instance when the callback ends.
       */
      virtual void release() = 0;
    };

    /**
     @brief The class for retrieving the attributes of a channel member.
     */
    class IChannelMember
    {
    protected:
      virtual ~IChannelMember() {}
    public:
        
      /**
       Retrieves the user ID of a user in the channel.
       
       @return User ID of a user in the channel.
       */
      virtual const char * getUserId() const = 0;
        
      /**
       Retrieves the channel ID of the user.
       
       @return Channel ID of the user.
       */
      virtual const char * getChannelId() const = 0;
        
      /**
       Releases all resources used by the IChannelMember instance.
       */
      virtual void release() = 0;
    };
    
    /**
     @brief The data structure holding an RTM user and his/her online status.
     */
    struct PeerOnlineStatus
    {
        
      /**
       The user ID of the peer.
       */
      const char* peerId;
        
      /**
       The online status of the peer.
       
       - true: The user is online.
       - false: The user is offline.
       */
      bool isOnline;
    };

    /**
     @brief Callbacks for the Agora RTM channel methods.
     */
    class IChannelEventHandler
    {
    public:

      virtual ~IChannelEventHandler()
      {
      }
        
      /**
       Occurs when successfully joining a channel.

       When the local user calls the \ref agora::rtm::IChannel::join "join" method and successfully joins the channel:
       - The SDK triggers this callback;
       - All remote users in the channel receive the \ref agora::rtm::IChannelEventHandler::onMemberJoined "onMemberJoined" callback.
       */
      virtual void onJoinSuccess()
      {
      }
        
      /**
       Occurs when failing to join a channel.

       The local user receives this callback when the \ref agora::rtm::IChannel::join "join" method call fails.
       
       @param errorCode The error code. See #JOIN_CHANNEL_ERR.
       */
      virtual void onJoinFailure(JOIN_CHANNEL_ERR errorCode)
      {
          (JOIN_CHANNEL_ERR) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IChannel::leave "leave" method call.
       
       @param errorCode The error code. See #LEAVE_CHANNEL_ERR.
       */
      virtual void onLeave(LEAVE_CHANNEL_ERR errorCode)
      {
          (LEAVE_CHANNEL_ERR) errorCode;
      }

      /**
       Occurs when receiving a channel message.

       @param userId The message sender.
       @param message The received channel message. See IMessage.
       */
      virtual void onMessageReceived(const char *userId, const IMessage *message)
      {
          (const char *) userId;
          (IMessage *) message;
      }

      /**
       Returns the result of the \ref agora::rtm::IChannel::sendMessage "sendMessage" method call.

       @param messageId The ID of the sent channel message.
       @param state The error codes. See #CHANNEL_MESSAGE_ERR_CODE.
       */
      virtual void onSendMessageResult(long long messageId, CHANNEL_MESSAGE_ERR_CODE state)
      {
          (long long) messageId;
          (CHANNEL_MESSAGE_ERR_CODE) state;
      }
        
      /**
       Occurs when a remote user joins the channel.

       When a remote user calls the \ref agora::rtm::IChannel::join "join" method and receives the \ref agora::rtm::IChannelEventHandler::onJoinSuccess "onJoinSuccess" callback (successfully joins the channel), the local user receives this callback.
       
       @note This callback is disabled when the number of the channel members exceeds 512.

       @param member The user joining the channel. See IChannelMember.
       */
      virtual void onMemberJoined(IChannelMember *member)
      {
          (IChannelMember *) member;
      }
        
      /**
       Occurs when a remote member leaves the channel.

       When a remote member in the channel calls the \ref agora::rtm::IChannel::leave "leave" method and receives the the \ref agora::rtm::IChannelEventHandler::onLeave "onLeave (LEAVE_CHANNEL_ERR_OK)" callback, the local user receives this callback.
       
       @note This callback is disabled when the number of the channel members exceeds 512.

       @param member The channel member that leaves the channel. See IChannelMember.
       */
      virtual void onMemberLeft(IChannelMember *member)
      {
          (IChannelMember *) member;
      }
        
      /**
       Returns the result of the \ref agora::rtm::IChannel::getMembers "getMembers" method call.
       
       When the method call succeeds, the SDK returns the member list of the channel.

       @param members The member list. See IChannelMember.
       @param userCount The number of members.
       @param errorCode Error code. See #GET_MEMBERS_ERR for the error codes.
       */
      virtual void onGetMembers(IChannelMember **members, int userCount, GET_MEMBERS_ERR errorCode)
      {
          (IChannelMember **) members;
          (int) userCount;
      }
    };

    /**
     @brief Agora RTM channel methods.
     */
    class IChannel
    {
    protected:
      virtual ~IChannel() {}
    public:
//            virtual void setEventHandler(IChannelEventHandler *eventHandler) = 0;

      /**
       Joins a channel.
       
       @note You can join a maximum of 20 RTM channels at the same time. When the number of the channels you join exceeds the limit, you receive the \ref agora::rtm::JOIN_CHANNEL_ERR "JOIN_CHANNEL_ERR_FAILURE" error code.

       - If this method call succeeds:
          - The local user receives the \ref agora::rtm::IChannelEventHandler::onJoinSuccess "onJoinSuccess" callback.
          - All remote users receive the \ref agora::rtm::IChannelEventHandler::onMemberJoined "onMemberJoined" callback.
       - If this method call fails, the local user receives the \ref agora::rtm::IChannelEventHandler::onJoinFailure "onJoinFailure" callback. See #JOIN_CHANNEL_ERR for the error codes.
       @return
       - 0: Success.
       - &ne;0: Failure. See #JOIN_CHANNEL_ERR for the error code.
      */
      virtual int join() = 0;
 
      /**
       Leaves a channel.

       - If this method call succeeds:
          - The local user receives the \ref agora::rtm::IChannelEventHandler::onLeave "onLeave" callback with the LEAVE_CHANNEL_ERR_OK state.
          - All remote users receive the \ref agora::rtm::IChannelEventHandler::onMemberLeft "onMemberLeft" callback.
       - If this method call fails, the local user receives the \ref agora::rtm::IChannelEventHandler::onLeave "onLeave" callback with an error code. See \ref agora::rtm::LEAVE_CHANNEL_ERR "LEAVE_CHANNEL_ERR" for the error codes.
       @return
       - 0: Success.
       - &ne;0: Failure. See #LEAVE_CHANNEL_ERR for the error code.
       */
      virtual int leave() = 0;
 
      /**
       Allows a channel member to send a message to all members in the channel.

       If this method call succeeds:
       - The \ref agora::rtm::IChannelEventHandler::onSendMessageResult "onSendMessageResult" callback returns the result.
       - All remote users in the channel receive the \ref agora::rtm::IChannelEventHandler::onMessageReceived "onMessageReceived" callback.
       
       @note You can send messages, including peer-to-peer and channel messages, at a maximum speed of 60 queries per second.

       @param message The message to be sent. See IMessage.
       @return
       - 0: Success.
       - &ne;0: Failure. See #CHANNEL_MESSAGE_ERR_CODE for the error code.
       */
      virtual int sendMessage(const IMessage *message) = 0;

      /**
       Retrieves the channel ID.

       @return The channel ID of the channel.
       */
      virtual const char *getId() const = 0;
        
        
        
      /**
       Retrieves a member list of the channel.

       The \ref agora::rtm::IChannelEventHandler::onGetMembers "onGetMembers" callback returns the result of this method call.
       
       @note You can call this method at a maximum speed of five queries every two seconds.

       @return
       - 0: Success.
       - &ne;0: Failure. See #GET_MEMBERS_ERR for the error code.
       */
      virtual int getMembers() = 0;

      // sync_call
        
      /**
       Releases all resources used by the IChannel instance.
       */
      virtual void release() = 0;
    };

    class IRtmServiceEventHandler
    {
    public:
      virtual ~IRtmServiceEventHandler()
      {
      }

      /**
       Occurs when a user logs in the Agora RTM system.

       The local user receives this callback when the \ref agora::rtm::IRtmService::login "login" method call succeeds.
       */
      virtual void onLoginSuccess() {}
        
      /**
       Occurs when a user fails to log in the Agora RTM system.

       The local user receives this callback when the \ref agora::rtm::IRtmService::login "login" method call fails. See \ref agora::rtm::LOGIN_ERR_CODE "LOGIN_ERR_CODE" for the error codes.
       */
      virtual void onLoginFailure(LOGIN_ERR_CODE errorCode)
      {
          (LOGIN_ERR_CODE) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::renewToken "renewToken" method call.
       
       @param token Your new token.
       @param errorCode The error code. See #RENEW_TOKEN_ERR_CODE.
       */
      virtual void onRenewTokenResult(const char* token, RENEW_TOKEN_ERR_CODE errorCode)
      {
          (const char*) token;
          (RENEW_TOKEN_ERR_CODE) errorCode;
      }
 
      /**
       Occurs when the token expires.
       
       This callback occurs when the token expires and reminds the user to renew it. When receiving this callback, generate a new Token on the server and call the \ref agora::rtm::IRtmService::renewToken "renewToken" method to pass the new Token on to the server.
       */
      virtual void onTokenExpired()
      {
      }

      /**
       Occurs when a user logs out of the Agora RTM system.

       The local user receives this callback when the SDK calls the \ref agora::rtm::IRtmService::logout "logout" method. See \ref agora::rtm::LOGOUT_ERR_CODE "LOGOUT_ERR_CODE" for the states.
       */
      virtual void onLogout(LOGOUT_ERR_CODE errorCode)
      {
          (LOGOUT_ERR_CODE) errorCode;
      }
        
      /**
       Occurs when the connection state changes between the SDK and the Agora RTM system.

       @param state The new connection state. See #CONNECTION_STATE.
       @param reason The reason for the connection state change. See #CONNECTION_CHANGE_REASON.
       */
      virtual void onConnectionStateChanged(CONNECTION_STATE state, CONNECTION_CHANGE_REASON reason)
      {
      }
        
      /**
       Returns the result of the \ref agora::rtm::IRtmService::sendMessageToPeer "sendMessageToPeer" method call.

       @param messageId The ID of the sent message.
       @param errorCode The peer-to-peer message state. See #PEER_MESSAGE_ERR_CODE.

       */
      virtual void onSendMessageResult(long long messageId, PEER_MESSAGE_ERR_CODE errorCode)
      {
          (long long) messageId;
          (PEER_MESSAGE_ERR_CODE) errorCode;
      }
    
      /**
       Occurs when receiving a peer-to-peer message.

       The receiver receives this callback while the sender receives the \ref agora::rtm::IRtmServiceEventHandler::onSendMessageResult "onSendMessageResult" callback when the \ref agora::rtm::IRtmService::sendMessageToPeer "sendMessageToPeer" method call succeeds.

       @param peerId User ID of the sender.
       @param message The message sent by the sender. See IMessage.
       */
      virtual void onMessageReceivedFromPeer(const char *peerId, const IMessage *message)
      {
          (char *) peerId;
          (IMessage *) message;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::queryPeersOnlineStatus "queryPeersOnlineStatus" method call.

       @param requestId The unique ID of this request.
       @param peersStatus The online status of the peer. See PeerOnlineStatus.
       @param peerCount The number of the queried peers.
       @param errorCode Error Codes. See #QUERY_PEERS_ONLINE_STATUS_ERR for the error code.
       */
      virtual void onQueryPeersOnlineStatusResult(long long requestId, const PeerOnlineStatus* peersStatus , int peerCount, QUERY_PEERS_ONLINE_STATUS_ERR errorCode)
      {
          (long long) requestId; 
          (const PeerOnlineStatus*) peersStatus;
          (int) peerCount;
          (QUERY_PEERS_ONLINE_STATUS_ERR) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual void onSetLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
          (long long) requestId;
          (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual void onAddOrUpdateLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual void onDeleteLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual void onClearLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
          (long long) requestId;
          (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" or \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" method call.

       @param requestId The unique ID of this request.
       @param userId The user ID of the specified user.
       @param attributes An array of the returned attributes. See RtmAttribute.
       @param numberOfAttributes Number of the user's attributes
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual void onGetUserAttributesResult(long long requestId, const char* userId, const RtmAttribute* attributes, int numberOfAttributes, ATTRIBUTE_OPERATION_ERR errorCode)
      {
          (long long) requestId;
          (const RtmAttribute*) attributes;
          (int) numberOfAttributes;
          (ATTRIBUTE_OPERATION_ERR) errorCode;
      }
    };

    class IRtmCallManager;
    class IRtmCallEventHandler;
      
    /**
     @brief Output log filter level.
     */
    enum LOG_FILTER_TYPE
    {
		
      /**
       0: Do not output any log information.
	   */
      LOG_FILTER_OFF = 0,
	  
      /**
       0x000f: Output CRITICAL, ERROR, WARNING, and INFO level log information.
	   */
      LOG_FILTER_INFO = 0x000f,
	  
      /**
       0x000e: Output CRITICAL, ERROR, and WARNING level log information.
	   */
      LOG_FILTER_WARN = 0x000e,
	  
      /**
       0x000c: Output CRITICAL and ERROR level log information.
	   */
      LOG_FILTER_ERROR = 0x000c,
	  
      /**
       0x0008: Output CRITICAL level log information.
	   */
      LOG_FILTER_CRITICAL = 0x0008,
	  
      LOG_FILTER_MASK = 0x80f,
    };

    class IRtmService
    {
    protected:
      virtual ~IRtmService() {}
    public:
        
      /**
       Initializes an IRtmService instance.

       All methods in the IRtmService class are executed asynchronously.

       @param appId The App ID issued by Agora to you. Apply for a new App ID from Agora if it is missing from your kit.
       @param eventHandler An IRtmServiceEventHandler object that invokes callbacks to be passed to the application on Agora RTM SDK runtime events.
       @return
       - 0: Success.
       - < 0: Failure. 
       */
      virtual int initialize(const char *appId, IRtmServiceEventHandler *eventHandler) = 0;
        
      /**
       Adds an IRtmServiceEventHandler object.

       @param eventHandler An IRtmServiceEventHandler object.
       */
      virtual void addEventHandler(IRtmServiceEventHandler *eventHandler) = 0;
        
      /**
       Removes an IRtmServiceEventHandler object.

       @param eventHandler An IRtmServiceEventHandler object.
       */
      virtual void removeEventHandler(IRtmServiceEventHandler *eventHandler) = 0;

      /**
       Releases all resources used by the IRtmService instance.
       */
      virtual void release(bool sync = false) = 0;

      /**
       Logs in the Agora RTM system.

       - If this method call succeeds, the local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginSuccess "onLoginSuccess" callback.
       - If this method call fails, the local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginFailure "onLoginFailure" callback. See \ref agora::rtm::LOGIN_ERR_CODE "LOGIN_ERR_CODE" for the error codes.
              
       **Note**
       - If you log in with the same user ID from a different instance, you will be kicked out of your previous login and removed from previously joined channels.
       - The call frequency limit for this method is 2 queries per second.
       - Only after you successfully call this method (when the local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginSuccess "onLoginSuccess" callback), can you call the key RTM methods except:
            - \ref agora::rtm::IRtmService::createChannel "createChannel"
            - \ref agora::rtm::IRtmService::createMessage "createMessage"
            - \ref agora::rtm::IMessage::setText "setText"
            - \ref agora::rtm::IRtmService::getRtmCallManager "getRtmCallManager"
            - \ref agora::rtm::IRtmCallManager::createLocalCallInvitation "createLocalCallInvitation"

       @param token Token used to log in the Agora RTM system and used when dynamic authentication is enabled. Set @p token as @p nil in the integration and test stages.
       @param userId The user ID of the user logging in Agora's RTM system. The string must not exceed 64 bytes in length. It cannot be empty, null or "null".
       Supported characters:
       - The 26 lowercase English letters: a to z
       - The 26 uppercase English letters: A to Z
       - The 10 numbers: 0 to 9
       - Space
       - "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ","
       @return
       - 0: Success.
       - &ne;0: Failure. See #LOGIN_ERR_CODE for the error code.
       */
      virtual int login(const char *token, const char *userId) = 0;

      /**
       Logs out of the Agora RTM system.

       The local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLogout "onLogout" callback. See \ref agora::rtm::LOGOUT_ERR_CODE "LOGIN_ERR_CODE" for the states.
       @return
       - 0: Success.
       - &ne;0: Failure. See #LOGOUT_ERR_CODE for the error code.
       */
      virtual int logout() = 0;
  
      /**
       Renews the token.
       
       You are required to renew your Token when receiving the \ref agora::rtm::IRtmServiceEventHandler::onTokenExpired "onTokenExpired" callback, and the \ref agora::rtm::IRtmServiceEventHandler::onRenewTokenResult "onRenewTokenResult" callback returns the result of this method call. The call frequency limit for this method is 2 queries per second.
       
       @param token Your new token.
       @return
       - 0: Success.
       - &ne;0: Failure. See #RENEW_TOKEN_ERR_CODE for the error code.
       */
      virtual int renewToken(const char* token) = 0;

      /**
       Sends a peer-to-peer message to a specified user (receiver).
       
       @note We do not recommend using this method to send a peer-to-peer message. Use \ref agora::rtm::IRtmService::sendMessageToPeer(const char *peerId, const IMessage *message, const SendMessageOptions &options) "sendMessageToPeer" instead.

       If this method call succeeds:
       - The sender receives the \ref agora::rtm::IRtmServiceEventHandler::onSendMessageResult "onSendMessageResult" callback.
       - The receiver receives the \ref agora::rtm::IRtmServiceEventHandler::onMessageReceivedFromPeer "onMessageReceivedFromPeer" callback.
       
       @note You can send messages, including peer-to-peer and channel messages, at a maximum speed of 60 queries per second.

       @param peerId User ID of the receiver.
       @param message The message to be sent. For information about creating a message, see IMessage.
       @return
       - 0: Success.
       - &ne;0: Failure. See #PEER_MESSAGE_ERR_CODE for the error code.
       */
      virtual int sendMessageToPeer(const char *peerId, const IMessage *message) = 0;
        
      /**
       Sends an (offline) peer-to-peer message to a specified user (receiver).
  
       This method allows you to send a message to a specified user when he/she is offline. If you set a message as an offline message and the specified user is offline when you send it, the RTM server caches it. Please note that for now we only cache 200 offline messages for up to seven days for each receiver. When the number of the cached messages reaches this limit, the newest message overrides the oldest one.
        
       If this method call succeeds:
       - The sender receives the \ref agora::rtm::IRtmServiceEventHandler::onSendMessageResult "onSendMessageResult" callback.
       - The receiver receives the \ref agora::rtm::IRtmServiceEventHandler::onMessageReceivedFromPeer "onMessageReceivedFromPeer" callback.
       
       @note You can send messages, including peer-to-peer and channel messages, at a maximum speed of 60 queries per second.

       @param peerId User ID of the receiver. The string must not exceed 64 bytes in length. It cannot be empty, null, or "null".
       
       Supported characters:
       
       - The 26 lowercase English letters: a to z
       - The 26 uppercase English letters: A to Z
       - The 10 numbers: 0 to 9
       - Space
       - "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ","
       
       @param message The message to be sent. For information about creating a message, See IMessage.
       @param options Options when sending the message to a peer. See \ref agora::rtm::SendMessageOptions "SendMessageOptions".
       @return
       - 0: Success.
       - &ne;0: Failure. See #PEER_MESSAGE_ERR_CODE for the error code.
       */
      virtual int sendMessageToPeer(const char *peerId, const IMessage *message, const SendMessageOptions& options) = 0;

      /**
       Creates an Agora RTM channel.

       - If the method call succeeds, the SDK returns an IChannel instance.
       - If this method call fails, the SDK returns @p null.

       @note You can create multiple channels in an IRtmService instance. But you can only join a maximum of 20 channels at the same time. As a good practice, we recommend calling the \ref agora::rtm::IChannel::release "release" method to release all resources of an RTM channel that you no longer use.

       @param channelId The unique channel name. Must be less than 64 bytes in length.
       Supported characters:
       
       - The 26 lowercase English letters: a to z
       - The 26 uppercase English letters: A to Z
       - The 10 numbers: 0 to 9
       - Space
       - "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ","
       
       **Note** A @p channelId cannot be empty or null.
       @param eventHandler See IChannelEventHandler.
       @return
       - An \ref agora::rtm::IChannel "IChannel" object: Success.
       - null: Failure. Possible reasons: The @p channelId is invalid, or a channel with the same @p channelId already exists.
       */
      virtual IChannel * createChannel(const char *channelId, IChannelEventHandler *eventHandler) = 0;
        
      /**
       Gets an \ref agora::rtm::IRtmCallManager "IRtmCallManager" object.
     
       Each \ref agora::rtm::IRtmService "IRtmService" object corresponds to a unique \ref agora::rtm::IRtmCallManager "IRtmCallManager" object. That said, the \ref agora::rtm::IRtmService "IRtmService" object is \ref agora::rtm::IRtmCallManager "IRtmCallManager"-specific.
     
       @note When you do no use \ref agora::rtm::IRtmCallManager "IRtmCallManager", call the \ref agora::rtm::IRtmCallManager::release "release" method to release all of its resources.
     
       @param eventHandler An \ref agora::rtm::IRtmCallEventHandler "IRtmCallEventHandler" object.
       @return An \ref agora::rtm::IRtmCallManager "IRtmCallManager" object.
       */
      virtual IRtmCallManager* getRtmCallManager(IRtmCallEventHandler*eventHandler) = 0;
        
      /**
       Creates an \ref agora::rtm::IMessage "IMessage" instance.
       
       @note
       - An \ref agora::rtm::IMessage "IMessage" instance can be used either for a channel message or for a peer-to-peer message.
       - When you no longer need an \ref agora::rtm::IMessage "IMessage" instance, please call the \ref agora::rtm::IMessage::release "release" method to release all resources that it is using.
       
       @return An \ref agora::rtm::IMessage "IMessage" instance.
       */
      virtual IMessage *createMessage() = 0;
        
      /**
       Provides the technical preview functionalities or special customizations by configuring the SDK with JSON options.
       
       @note The JSON options are not public by default. Agora is working on making commonly used JSON options public in a standard way. Contact [support@agora.io](mailto:support@agora.io) for more information.
       
       @param parameters SDK options in the JSON format.
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setParameters(const char* parameters) = 0;
//            virtual int getParameters(const char* key, any_document_t& result) = 0;
 
      /**
       Queries the online status of the specified users.
       
       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onQueryPeersOnlineStatusResult "onQueryPeersOnlineStatusResult" callback.
       
       @param peerIds[] An array of the specified user IDs.
       @param peerCount Length of the list.
       @param requestId The requester's user ID.
       @return
       - 0: Success.
       - &ne;0: Failure. See #QUERY_PEERS_ONLINE_STATUS_ERR for the error code.
       */
      virtual int queryPeersOnlineStatus(const char* peerIds[], int peerCount, long long& requestId) = 0;

      /**
       Substitutes the local user's attributes with new ones.
       
       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onSetLocalUserAttributesResult "onSetLocalUserAttributesResult" callback.
       
       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the call frequency limit is 10 queries every five seconds.

       @param attributes The new attributes. See RtmAttribute.
       @param numberOfAttributes The number of the attributes.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual int setLocalUserAttributes(const RtmAttribute* attributes, int numberOfAttributes, long long &requestId) = 0;

      /**
       Adds or updates the local user's attribute(s).
       
       This method updates the local user's attribute(s) if it finds that the attribute(s) has/have the same key(s), or adds attribute(s) to the local user if it does not.
       
       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onAddOrUpdateLocalUserAttributesResult "onAddOrUpdateLocalUserAttributesResult" callback.
       
       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the call frequency limit is 10 queries every five seconds.

       @param attributes The attrubutes to be added or updated. See RtmAttribute.
       @param numberOfAttributes The number of the attributes.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual int addOrUpdateLocalUserAttributes(const RtmAttribute* attributes, int numberOfAttributes, long long &requestId) = 0;

      /**
       Deletes the local user's attributes by attribute keys.
       
       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onDeleteLocalUserAttributesResult "onDeleteLocalUserAttributesResult" callback.
       
       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the call frequency limit is 10 queries every five seconds.

       @param attributeKeys An array of the attribute keys to be deleted.
       @param numberOfKeys Number of the attribute keys.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual int deleteLocalUserAttributesByKeys(const char* attributeKeys[], int numberOfKeys, long long &requestId) = 0;
        
      /**
       Clears all attributes of the local user.
       
       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onClearLocalUserAttributesResult "onClearLocalUserAttributesResult" callback.
       
       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the call frequency limit is 10 queries every five seconds.

       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual int clearLocalUserAttributes(long long &requestId) = 0;
  
      /**
       Gets all attributes of a specified user.
       
       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onGetUserAttributesResult "onGetUserAttributesResult" callback.
       
       @note For \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" and \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" taken together, the call frequency limit is 40 queries every five seconds.

       @param userId The user ID of the specified user.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual int getUserAttributes(const char* userId, long long &requestId) = 0;
        
      /**
       Gets the attributes of a specified user by attribute keys.
       
       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onGetUserAttributesResult "onGetUserAttributesResult" callback.
       
       @note For \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" and \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" taken together, the call frequency limit is 40 queries every five seconds.

       @param userId The user ID of the specified user.
       @param attributeKeys An array of the attribute keys.
       @param numberOfKeys The number of the attribute keys.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error code.
       */
      virtual int getUserAttributesByKeys(const char* userId, const char* attributeKeys[], int numberOfKeys, long long &requestId) = 0;
        
      /**
       Specifies the default path to the SDK log file.

       @param logfile The absolute file path to the log file. The string of the log file is in UTF-8. 
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setLogFile(const char* logfile) = 0;
        
      /**
       Sets the output log level of the SDK.
       
       You can use one or a combination of the filters. The log level follows the sequence of OFF, CRITICAL, ERROR, WARNING, and INFO. Choose a level to see the logs preceding that level. If, for example, you set the log level to WARNING, you see the logs within levels CRITICAL, ERROR, and WARNING.

       @param filter The log filter level. See #LOG_FILTER_TYPE.
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setLogFilter(LOG_FILTER_TYPE filter) = 0;
      
      /**
       Sets the log file size (KB).
       
       The SDK has two log files, each with a default size of 512 KB. If you set fileSizeInBytes as 1024 KB, the SDK outputs log files with a total maximum size of 2 MB. If the total size of the log files exceeds the set value, the new output log files overwrite the old output log files.

       @param fileSizeInKBytes The SDK log file size (KB).
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setLogFileSize(int fileSizeInKBytes) = 0;
    };
  }
}
