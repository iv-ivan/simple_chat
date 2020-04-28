//
//  client.cpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#include "client.h"
#include "common.h"
#include <future>

using namespace std;

void TClient::Run() {
    int sock = createSocket(ServerPort, false);
    if (sock == -1) {
        cerr << "Can't connect to " << ServerPort << endl;
        return;
    }
    
    auto sender = std::async(std::launch::async, &TClient::SendingLoop, this, sock);

    ProcessMessage(sock);

    sender.wait();
}

void TClient::SendingLoop(int sendSock) {
    cout << "Sending to " << ServerPort << endl;
    
    string connectString = "connect " + Name + "$";
    send(sendSock, connectString.data(), connectString.size(), 0);

    while (true) {
        string input;
        getline(cin, input);

        cout << "Sending to " << ServerPort << endl;

        input = "send " + Name + " " + input + "$";
        send(sendSock, input.data(), input.size(), 0);
    }
}

void TClient::ProcessMessage(int sock) {
    constexpr int N = 1024;
    char buf[N];
    string message;
    while (true) {
        long bytesSize = recv(sock, buf, N, 0);
        if (bytesSize <= 0) {
            break;
        }
        string tmpMessage = string(buf);
        while (tmpMessage.find("$") != string::npos) {
            message += tmpMessage.substr(0, tmpMessage.find("$"));
            tmpMessage = tmpMessage.substr(tmpMessage.find("$") + 1, tmpMessage.size() - tmpMessage.find("$") - 1);
        
            ProcessCommand(message, sock);
            message.clear();
        }
        message += tmpMessage;
        memset(&buf[0], 0, N);
    }
}

void TClient::ProcessCommand(const string& commandMessage, int sock) {
    string_view other = commandMessage;
    string_view command = nextTok(other, ' ');

    if (command == "history") {
        // won't work until we proper shutdown socket on destructor (should make recv() non blocking)
        string_view history = other;
        Print("UPDATES: " + string(history));
    } else if (command == "message") {
        string_view msg = other;
        Print(string(msg));
    } else if (command == "ok") {
        return;
    } else {
        cerr << "bad message:" << commandMessage << "; command:" << command << ";" << endl;
        return;
    }

    send(sock, string("ok$").data(), 3, 0);
}

void TClient::Print(const string& txt) {
    cout << txt << endl;
}
