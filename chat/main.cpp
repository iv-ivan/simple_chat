//
//  main.cpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#include <iostream>
#include <string>
#include "server.h"
#include "client.h"

using namespace std;

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        cerr << "mode<server, client> options";
        return 1;
    }

    const size_t port = std::stoi(argv[2]);

    if (strcmp(argv[1], "server") == 0) {
        if (argc < 3) {
            cerr << "server port";
            return 1;
        }
        TServer s(port);
        s.Run();
    } else if (strcmp(argv[1], "client") == 0) {
        if (argc < 4) {
            cerr << "client server_port client_name";
            return 1;
        }
        const string myName = argv[3];
        TClient c(port, myName);
        c.Run();
    }

    return 0;
}
