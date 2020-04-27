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
#include <string>

class TClient {
public:
    TClient(size_t port, size_t serverPort, std::string clientName)
        : Port(port)
        , ServerPort(serverPort)
        , Name(clientName)
    {}
    
    void Run();
private:
    void ProcessMessage(int sock);
    void SendingLoop();
    
    void Print(const std::string& txt);
private:
    size_t Port;
    size_t ServerPort;
    std::string Name;
};

#endif /* client_hpp */
