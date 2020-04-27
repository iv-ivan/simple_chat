//
//  client.hpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#ifndef client_hpp
#define client_hpp

#include <iostream>

class TClient {
public:
    TClient(size_t port): Port(port) {
    }
    
    void Run();
private:
    size_t Port;
};

#endif /* client_hpp */
