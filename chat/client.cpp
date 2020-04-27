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
    int listenSock = createSocket(Port, true);
    if (listenSock == -1) {
        cerr << "Can't listen on " << Port << endl;
        return;
    }
    
    auto sender = std::async(std::launch::async, &TClient::SendingLoop, this);
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

void TClient::SendingLoop() {
    int sendSock = createSocket(ServerPort, false);
    if (sendSock == -1) {
        cerr << "Can't send to " << ServerPort << endl;
        return;
    }
    cout << "Sending to " << ServerPort << endl;
    
    string connectString = "connect " + Name + " " + to_string(Port);
    send(sendSock, connectString.data(), connectString.size() + 1, 0);
    shutdown(sendSock, 2);

    while (true) {
        string input;
        getline(cin, input);
        if (input == "q") {
            break;
        }
        
        int sendSock = createSocket(ServerPort, false);
        if (sendSock == -1) {
            cerr << "Can't send to " << ServerPort << endl;
            return;
        }
        cout << "Sending to " << ServerPort << endl;

        input = "send " + Name + " " + input;
        send(sendSock, input.data(), input.size() + 1, 0);
        shutdown(sendSock, 2);
    }
    
    exit(0);
}

void TClient::ProcessMessage(int sock) {
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

    if (command == "history") {
        string_view history = other;
        Print("UPDATES: " + string(history));
    } else if (command == "message") {
        string_view msg = other;
        Print(string(msg));
    } else if (command == "ok") {
    } else {
        cerr << "bad message:" << message << "; command:" << command << ";" << endl;
        shutdown(sock, 2);
        return;
    }

    send(sock, string("ok").data(), 3, 0);
    shutdown(sock, 2);
}

void TClient::Print(const string& txt) {
    cout << txt << endl;
}
