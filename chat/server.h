//
//  server.hpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#ifndef server_hpp
#define server_hpp

#include <iostream>

class TServer {
public:
    TServer(size_t port): Port(port) {
    }
    
    void Run();
private:
    size_t Port;
};

#endif /* server_hpp */
