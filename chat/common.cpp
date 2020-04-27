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

int createSocket(size_t port) {
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "error socket";
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(sock, (const struct sockaddr *)&addr, static_cast<socklen_t>(sizeof(addr))) < 0) {
        cerr << "error bind";
        return -1;
    }
    listen(sock, 1024);
    
    return sock;
}
