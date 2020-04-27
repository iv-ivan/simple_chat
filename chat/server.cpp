//
//  server.cpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#include "server.h"
#include "common.h"

void TServer::Run() {
    int socket = createSocket(Port);
    socket += 1;
}
