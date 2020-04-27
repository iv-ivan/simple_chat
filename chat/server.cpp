//
//  server.cpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#include "server.h"
#include "common.h"

using namespace std;

void TServer::Run() {
    int listenSock = createSocket(Port, true);
    if (listenSock == -1) {
        cerr << "Can't listen on " << Port << endl;
        return;
    }
    cout << "Listening on " << Port << endl;

    while (true) {
        int sock = accept(listenSock, NULL, NULL);
        if (sock < 0) {
            cerr << "Bad accept " << endl;
            perror("Accept : ");
            continue;
        }

        ProcessMessage(sock);
    }
}

void TServer::ProcessMessage(int sock) {
    constexpr int N = 1024;
    char buf[N];
    string message;
    while (true) {
        size_t bytes_read = recv(sock, buf, N, 0);
        if (bytes_read <= 0) {
            break;
        }
        message.append(buf);
    }
    
    //cout << "Message:" << message << endl;
    string_view other = message;
    string_view command = nextTok(other, ' ');

    if (command == "connect") {
        string_view name = nextTok(other, ' ');
        size_t port = stoi(string(other));
        RegisterClient(string(name), port);
        FetchNewMessages(string(name));
    } else if (command == "send") {
        string_view senderName = nextTok(other, ' ');
        string_view receiverName = nextTok(other, ' ');
        SendMessage(string(senderName), string(receiverName), string(other));
    } else if (command == "ok") {
    } else {
        cerr << "bad message:" << message << "; command:" << command << ";" << endl;
        shutdown(sock, 2);
        return;
    }

    send(sock, string("ok").c_str(), 3, 0);
    shutdown(sock, 2);
}

void TServer::RegisterClient(const std::string& name, size_t port) {
    Clients[name] = port;
}

void TServer::SendMessage(const std::string& senderName, const std::string& receiverName, const std::string& message) {
    if (Clients.count(receiverName) == 0) {
        return;
    }
    
    int sendSock = createSocket(Clients[receiverName], false);
    if (sendSock == -1) {
        cerr << "Can't send to " << receiverName << ", port:" << Clients[receiverName] << endl;
        MessageQueue[receiverName].push_back(message);
        return;
    }
    //cout << "Sending to " << receiverName << ", port:" << Clients[receiverName] << endl;
    
    string msgString = "message " + senderName + ": " + message;
    send(sendSock, msgString.data(), msgString.size() + 1, 0);
    shutdown(sendSock, 2);
}

void TServer::FetchNewMessages(const std::string& name) {
    if (MessageQueue.count(name) == 0) {
        return;
    }

    int sendSock = createSocket(Clients[name], false);
    if (sendSock == -1) {
        cerr << "Can't send to " << name << ", port:" << Clients[name] << endl;
        return;
    }
    //cout << "Sending to " << name << ", port:" << Clients[name] << endl;
    
    string historyString = "history ";
    for (const auto& message : MessageQueue[name]) {
        historyString += message + "\n";
    }
    send(sendSock, historyString.data(), historyString.size() + 1, 0);
    shutdown(sendSock, 2);
}
