#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <map>
#include <algorithm>

#include "IAgoraService.h"
#include "IAgoraRtmService.h"

//socket
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int MSG_MAXCHAR=600;
int Agora_PORT=9988;
int Jling_PORT=9987;
char IP_ADDRESS[20]="127.0.0.1";

string APP_ID = "0ccd38ee0fe749789a336f9901cf910b";

class RtmEventHandler: public agora::rtm::IRtmServiceEventHandler {
  public:
    RtmEventHandler() {}
    ~RtmEventHandler() {}

    virtual void onLoginSuccess() override {
        cout << "on login success" << endl;
    }

    virtual void onLoginFailure(agora::rtm::LOGIN_ERR_CODE errorCode) override {
        cout << "on login failure: errorCode = " << errorCode << endl;
    }

    virtual void onLogout(agora::rtm::LOGOUT_ERR_CODE errorCode) override {
        cout << "on login out" << endl;
    }

    virtual void onConnectionStateChanged(agora::rtm::CONNECTION_STATE state,
                        agora::rtm::CONNECTION_CHANGE_REASON reason) override {
        cout << "on connection state changed: state = " << state << endl;
    }

    virtual void onSendMessageResult(long long messageId,
                        agora::rtm::PEER_MESSAGE_ERR_CODE state) override {
        cout << "on send message messageId: " << messageId << " state: "
             << state << endl;
    }

    //RECV
    virtual void onMessageReceivedFromPeer(const char *peerId,
                        const agora::rtm::IMessage *message) override {
        //建立udp socket
        int socked_sg=socket(AF_INET,SOCK_DGRAM,0);
        if(socked_sg<0)
        {
            printf("creat socket failed!\n");
            exit(1);
        }
        struct sockaddr_in Agora_addr;
        memset(&Agora_addr,0,sizeof(Agora_addr)); //每个字节都用0填充
        Agora_addr.sin_family=AF_INET;//使用ipv4地址
        Agora_addr.sin_port=htons(Jling_PORT);//端口
        Agora_addr.sin_addr.s_addr=inet_addr(IP_ADDRESS);//IP地址
        unsigned int len=sizeof(Agora_addr);

        sendto(socked_sg,message->getText(),strlen(message->getText()),0,(struct sockaddr *)&Agora_addr,len);
        cout << "on message received from peer: peerId = " << peerId
             << " ,message = " << message->getText() <<" ,send port:"<<Agora_PORT<< endl;
        close(socked_sg);
    }
};

class ChannelEventHandler: public agora::rtm::IChannelEventHandler {
  public:
    ChannelEventHandler(string channel) {
        channel_ = channel;
    }
    ~ChannelEventHandler() {}

    virtual void onJoinSuccess() override {
        cout << "on join channel success" << endl;
    }

    virtual void onJoinFailure(agora::rtm::JOIN_CHANNEL_ERR errorCode) override{
        cout << "on join channel failure: errorCode = " << errorCode << endl;
    }

    virtual void onLeave(agora::rtm::LEAVE_CHANNEL_ERR errorCode) override {
        cout << "on leave channel: errorCode = " << errorCode << endl;
    }

    virtual void onMessageReceived(const char* userId,
                        const agora::rtm::IMessage *msg) override {
        cout << "receive message from channel: " << channel_.c_str()
             << " user: " << userId << " message: " << msg->getText()
             << endl;
    }

    virtual void onMemberJoined(agora::rtm::IChannelMember *member) override {
        cout << "member: " << member->getUserId() << " joined channel: "
             << member->getChannelId() << endl;
    }

    virtual void onMemberLeft(agora::rtm::IChannelMember *member) override {
        cout << "member: " << member->getUserId() << " lefted channel: "
             << member->getChannelId() << endl;
    }

    virtual void onGetMembers(agora::rtm::IChannelMember **members,
                    int userCount,
                    agora::rtm::GET_MEMBERS_ERR errorCode) override {
        cout << "list all members for channel: " << channel_.c_str()
             << " total members num: " << userCount << endl;
        for (int i = 0; i < userCount; i++) {
            cout << "index[" << i << "]: " << members[i]->getUserId();
        }
    }

    virtual void onSendMessageResult(long long messageId,
                    agora::rtm::CHANNEL_MESSAGE_ERR_CODE state) override {
        cout << "send messageId: " << messageId << " state: " << state << endl;
    }

    private:
        string channel_;
};

class Demo {
  public:
    Demo() {
        coreService_.reset(createAgoraService());
        if (!coreService_) {
            cout << "core service created failure!" << endl;
            exit(0);
        }

        if (coreService_->initialize(context_)) {
            cout << "core service initialize failure!" << endl;
            exit(0);
        }

        eventHandler_.reset(new RtmEventHandler());
        agora::rtm::IRtmService* p_rs = coreService_->createRtmService();
        rtmService_.reset(p_rs, [](agora::rtm::IRtmService* p) {
            p->release();
        });

        if (!rtmService_) {
            cout << "rtm service created failure!" << endl;
            exit(0);
        }

        if (rtmService_->initialize(APP_ID.c_str(), eventHandler_.get())) {
            cout << "rtm service initialize failure! appid invalid?" << endl;
            exit(0);
        }
    }
    ~Demo() {
        rtmService_->release();
    }

  public:
    bool login(string ID) {
        string userID=ID;
        cout << "自身ID:" <<userID<<endl;
        if (rtmService_->login(APP_ID.c_str(), userID.c_str())) {
            cout << "login failed!" << endl;
            return false;
        }
        return true;
    }

    void logout() {
        rtmService_->logout();
        cout << "log out!" << endl;
    }

    void p2pChat(const std::string& dst) {
        //建立udp socket

        cout<<"Start UDP Recv IP:"<<IP_ADDRESS<<",PORT:"<<Jling_PORT<<endl;
        int socked_fd=socket(AF_INET,SOCK_DGRAM,0);
        if(socked_fd<0)
        {
            printf("creat socket failed!\n");
            exit(1);
        }
        struct sockaddr_in addr_serv;
        memset(&addr_serv,0,sizeof(addr_serv)); //每个字节都用0填充
        addr_serv.sin_family=AF_INET;//使用ipv4地址
        addr_serv.sin_port=htons(Agora_PORT);//端口
        addr_serv.sin_addr.s_addr=inet_addr(IP_ADDRESS);//IP地址
        unsigned int len=sizeof(addr_serv);
        bind(socked_fd,(sockaddr *)&addr_serv,sizeof(addr_serv));//将socket与制定PORT和IP绑定

        string msg;
        while(true) {
            char MSG[MSG_MAXCHAR]={0};
            recvfrom(socked_fd,MSG,MSG_MAXCHAR,0,(sockaddr *)&addr_serv,&len);
            msg=(string)MSG;
            cout<<"Recv:"<<msg<<" From:"<<Jling_PORT<<endl;
            if (msg.compare("quit") == 0) {
                return;
            } else {
                sendMessageToPeer(dst, msg);
            }
        }
    }

    void groupChat(const std::string& channel) {
        string msg;
        channelEvent_.reset(new ChannelEventHandler(channel));
        agora::rtm::IChannel * channelHandler =
            rtmService_->createChannel(channel.c_str(), channelEvent_.get());
        if (!channelHandler) {
            cout << "create channel failed!" << endl;
        }
        channelHandler->join();
        while(true) {
            cout << "please input message you want to send, or input \"quit\" "
                 << " to leave groupChat, or input \"members\" to list members"
                 << endl;
            getline(std::cin, msg);
            if (msg.compare("quit") == 0) {
                channelHandler->leave();
                return;
            } else if (msg.compare("members") == 0) {
                channelHandler->getMembers();
            } else {
                sendMessageToChannel(channelHandler, msg);
            }
        }
    }

    void sendMessageToPeer(std::string peerID, std::string msg) {
        agora::rtm::IMessage* rtmMessage = rtmService_->createMessage();
        rtmMessage->setText(msg.c_str());
        int ret = rtmService_->sendMessageToPeer(peerID.c_str(),
                                        rtmMessage);
        rtmMessage->release();
        if (ret) {
            cout << "send message to peer failed! return code: " << ret
                 << endl;
        }
    }

    void sendMessageToChannel(agora::rtm::IChannel * channelHandler,
                            string &msg) {
        agora::rtm::IMessage* rtmMessage = rtmService_->createMessage();
        rtmMessage->setText(msg.c_str());
        channelHandler->sendMessage(rtmMessage);
        rtmMessage->release();
    }

    private:
        std::unique_ptr<agora::base::IAgoraService> coreService_;
        agora::base::AgoraServiceContext context_;
        std::unique_ptr<agora::rtm::IRtmServiceEventHandler> eventHandler_;
        std::unique_ptr<ChannelEventHandler> channelEvent_;
        std::shared_ptr<agora::rtm::IRtmService> rtmService_;
};

int main(int argc, const char * argv[]) {
    APP_ID = "0ccd38ee0fe749789a336f9901cf910b";
    int count=1;
    cout << "有"<< count <<"个RTM实例"<< "，被限制3个" << endl;

    std::vector<std::unique_ptr<Demo>> DemoList;
    std::vector<bool> loginStatus;
    for (int i = 0; i < count; i++) {
        std::unique_ptr<Demo> tmp;
        tmp.reset(new Demo());
        DemoList.push_back(std::move(tmp));
        loginStatus.push_back(false);
    }
    int index;
    while(true) {
	index=1;
	cout << "将要使用"<<index<<"个rtm实例"<< endl;
	string userID="1234";
        while(true) {
            if (!loginStatus[index-1]) {
                if (!DemoList[index-1]->login(userID))
                    continue;
                loginStatus[index-1] = true;
            }
            int choice = 1;
            cout<<"p2p chat\n";
            string dst="12345";
	    cout<<"目标用户id:"<< dst << endl;
            DemoList[index-1]->p2pChat(dst);
        }
        cout << "Quit the demo? yes/no" << endl;
        string input_quit;
        getline(std::cin, input_quit);
        if (input_quit.compare("yes") == 0) {
            break;
        }
    }

    exit(0);
}
