//
//  client.cpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#include "client.h"
#include "common.h"

void TClient::Run() {
    int socket = createSocket(Port);
    socket += 1;
}
