//
//  main.cpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#include <iostream>
#include "server.h"
#include "client.h"

using namespace std;

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        cerr << "mode<server, client> port";
        return 1;
    }

    size_t port = std::stoi(argv[1]);

    if (strcmp(argv[0], "server") == 0) {
        TServer s(port);
        s.Run();
    } else if (strcmp(argv[0], "client") == 0) {
        TClient c(port);
        c.Run();
    }

    return 0;
}
