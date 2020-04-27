//
//  common.cpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#include "common.h"
#include <iostream>

using namespace std;

int createSocket(size_t port, bool isServer) {
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "error socket";
        perror("Socket : ");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = isServer? htonl(INADDR_ANY) : htonl(INADDR_LOOPBACK);

    if (isServer) {
        if (bind(sock, (const struct sockaddr *)&addr, static_cast<socklen_t>(sizeof(addr))) < 0) {
            cerr << "error bind";
            perror("Bind : ");
            return -1;
        }
        listen(sock, 1024);
        
        return sock;
    } else {
        if (connect(sock, (struct sockaddr *)&addr, static_cast<socklen_t>(sizeof(addr))) < 0) {
            cerr << "error connect";
            perror("Connect : ");
            return -1;
        }
        return sock;
    }
}

string_view nextTok(string_view& s, const char del) {
    auto delPos = s.find(del);
    if (delPos == string_view::npos) {
        string_view tmp = s;
        s = string_view();
        return tmp;
    } else {
        string_view token = s.substr(0, delPos);
        s = s.substr(delPos + 1, s.size() - (delPos + 1));
        return token;
    }
}
