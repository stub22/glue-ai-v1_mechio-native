/* 
 * File:   main.cpp
 * Author: mechio
 *
 * Created on December 26, 2011, 12:32 PM
 */
/* 
 * File:   main.cpp
 * Author: matt
 *
 * Created on November 12, 2011, 5:49 PM
     */
    #include <stdlib.h>
    #include <string.h>

    #include "mechio/messaging/MessagingProvider.h"
    #include "mechio/messaging/MessageSender.h"
    #include "mechio/messaging/protocol/ServiceCommandRecord.h"
    #include "mechio/speech/protocol/SpeechRequestRecord.h"


    using namespace std;
    using namespace mechio;

    void sendCmd(MessageSender* sender, string command){
        ServiceCommandRecord cmd;
        cmd.command = command;
        cmd.timestampMillisecUTC = 1;
        cmd.sourceId = "source";
        cmd.destinationId = "destination";
        sender->send<ServiceCommandRecord>(&cmd, "application/service-command", true);
    }

    void sending(MessageSender* cmdSender, MessageSender* reqSender){
        while(1){
            cout << "Enter a phrase: ";
            char *cmd = new char[8192];
            cin.getline(cmd, 8191);
            if(strncmp(cmd, ":q", 3) == 0){
                sendCmd(cmdSender, "stop");
                return;
            }else if(strncmp(cmd, ":p", 3) == 0){
                sendCmd(cmdSender, "pause");
                continue;
            }else if(strncmp(cmd, ":r", 3) == 0){
                sendCmd(cmdSender, "resume");
                continue;
            }
            SpeechRequestRecord* req = new SpeechRequestRecord();
            req->requestSourceId.assign("source");
            req->speechServiceId.assign("destination");
            req->phrase.assign(cmd);
            req->timestampMillisecUTC = 1;
            reqSender->send<SpeechRequestRecord>(req);
        }
    }

    void startSpeechClient(string broker, string prefix){
        MessagingProvider* aqs = new MessagingProvider(broker.c_str());
        aqs->connect();
        string cmdstr(prefix.c_str());
        cmdstr.append("Command; {create: always, node: {type: queue}}");
        string reqstr(prefix.c_str());
        reqstr.append("Request; {create: always, node: {type: queue}}");
        MessageSender* cmd = aqs->createSender(cmdstr.c_str());
        MessageSender* req = aqs->createSender(reqstr.c_str());
        sending(cmd, req);
        cmd->close();
        req->close();
        aqs->disconnect();
    }

int main(int argc, char **argv) {
    string broker;
    string prefix;
    if(argc <= 1){
        broker = "127.0.0.1:5672";
        prefix = "speech";
    }else if(argc == 2){
        broker = argv[1];
        broker.append(":5672");
        prefix = "speech";
    }else{
        broker = argv[1];
        broker.append(":5672");
        prefix = argv[2];
    }
    startSpeechClient(broker, prefix);
    return 0;
}