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

        int optionValue = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, (void*)&optionValue, sizeof(optionValue)) < 0) {
            cerr << "Bad accept " << endl;
            perror("No SigPipe : ");
            continue;
        }

        const std::lock_guard<std::mutex> lock(ClientsMutex);
        ClientConnections[sock] = std::async(std::launch::async, &TServer::ProcessMessage, this, sock);
    }
}

void TServer::ProcessMessage(int sock) {
    constexpr int N = 1024;
    char buf[N];
    string message;
    while (true) {
        long bytesSize = recv(sock, buf, N, 0);
        if (bytesSize <= 0) {
            return;
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

void TServer::ProcessCommand(const string& commandMessage, int sock) {
    string_view other = commandMessage;
    string_view command = nextTok(other, ' ');

    if (command == "connect") {
        string_view name = nextTok(other, ' ');
        RegisterClient(string(name), sock);
        FetchNewMessages(string(name));
    } else if (command == "send") {
        string_view senderName = nextTok(other, ' ');
        string_view receiverName = nextTok(other, ' ');
        SendMessage(string(senderName), string(receiverName), string(other));
    } else if (command == "ok") {
        return;
    } else {
        cerr << "bad message:" << commandMessage << "; command:" << command << ";" << endl;
        return;
    }

    send(sock, string("ok$").data(), 3, 0);
}

void TServer::RegisterClient(const std::string& name, int sock) {
    const std::lock_guard<std::mutex> lock(ClientsMutex);
    if (Clients.count(name) > 0) {
        ClientConnections[Clients[name]].wait();
        ClientConnections.erase(Clients[name]);
    }
    Clients[name] = sock;
}

void TServer::SendMessage(const std::string& senderName, const std::string& receiverName, const std::string& message) {
    const std::lock_guard<std::mutex> lock(ClientsMutex);
    if (Clients.count(receiverName) == 0) {
        return;
    }
    
    string msgString = "message " + senderName + ": " + message + "$";

    long bytesSize = send(Clients[receiverName], msgString.data(), msgString.size(), 0);
    if (bytesSize <= 0) {
        MessageQueue[receiverName].push_back(senderName + ": " + message);
    }
}

void TServer::FetchNewMessages(const std::string& name) {
    const std::lock_guard<std::mutex> lock(ClientsMutex);
    if (MessageQueue.count(name) == 0) {
        return;
    }

    string historyString = "history ";
    for (const auto& message : MessageQueue[name]) {
        historyString += message + "\n";
    }
    historyString += "$";
    send(Clients[name], historyString.data(), historyString.size(), 0);
}
